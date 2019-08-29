import argparse
import os
import re
import subprocess
import sys
import tempfile

'''
Read file contents into whitespace-stripped line list.
'''
def extract_lines(f):
    return [l.strip() for l in f.readlines()]


'''
Parse .rel.text entries, ignoring relocations that are nonzero (matching
entries in .rel.dyn for dynamic relocation) if desired. Zero-valued relocations
are more likely missing extern references that must be resolved.
'''

def parse_rel_text(lines, ignore_nonzero_symbol_vals=False):
    relocations = []
    for i, l in enumerate(lines):
        if l.startswith("Relocation section '.rel.text'"):
            break

    for l in lines[i+2:]: # Ignore title line and column headers
        if l == "":
            break # no more relocation entries in current section
        items = l.split()

        if ignore_nonzero_symbol_vals and int(items[3], base=16) != 0:
            continue

        relocations.append({
            'offset': items[0],
            'info': items[1],
            'type': items[2],
            'symbol_val': items[3],
            'symbol_name': items[4]
        })

    return relocations


def parse_got_offsets(lines, relocations):
    # We're looking in the text section for the GOT offsets matching the relocations
    for i, l in enumerate(lines):
        if l == "Disassembly of section .text:":
            break

    for r in relocations:
        pattern = re.compile(r['offset'] + ":\s*([0-9a-f]{8})")
        for l in lines[i+1:]: # Ignore matched line
            if l.startswith("Disassembly of section"):
                break # iterated into a different section

            result = pattern.match(l)
            if result:
                r['got_offset'] = int(result.group(1), base=16)
                break


def find_symbol(lines, pattern):
    for i, l in enumerate(lines):
        result = pattern.match(l)
        if result:
            return i, result
    return 0, None


def parse_symbol_locations(lines, relocations):
    if not lines:
        return

    for r in relocations:
        pattern = re.compile("\.text\S*\."+r['symbol_name']+"\s*0x0*([0-9a-f]{8})")
        index, result = find_symbol(lines, pattern)
        if result:
            print("\t(1) Resolved symbol", r['symbol_name'], "to", result.group(1), "using line \n\t<", lines[index], ">")
            r['symbol_val'] = result.group(1)
            continue

        pattern = re.compile("\.text\S*"+r['symbol_name']+"\s*0x0*([0-9a-f]{8})")
        index, result = find_symbol(lines, pattern)
        if result:
            print("\t(2) Resolved symbol", r['symbol_name'], "to", result.group(1), "using line \n\t<", lines[index], ">")
            r['symbol_val'] = result.group(1)
            continue

        pattern = re.compile("\.text\S*\."+r['symbol_name']+"\Z")
        index, result = find_symbol(lines, pattern)
        if result:
            pattern2 = re.compile("\s*0x0*([0-9a-f]{8})")
            index2, result = find_symbol(lines[index+1:], pattern2)
            if result:
                print("\t(3) Resolved symbol", r['symbol_name'], "to", result.group(1), "using line \n\t<", lines[index], ">")
                r['symbol_val'] = result.group(1)
                continue

        pattern = re.compile("\.text\S*"+r['symbol_name']+"\Z")
        index, result = find_symbol(lines, pattern)
        if result:
            pattern2 = re.compile("\s*0x0*([0-9a-f]{8})")
            index2, result = find_symbol(lines[index+1:], pattern2)
            if result:
                print("\t(4) Resolved symbol", r['symbol_name'], "to", result.group(1), "using line \n\t<", lines[index], ">")
                r['symbol_val'] = result.group(1)
                continue

        pattern = re.compile("\s*0x0*([0-9a-f]{8})\s*"+r['symbol_name']+"\Z")
        index, result = find_symbol(lines, pattern)
        if result:
            print("\t(5) Resolved symbol", r['symbol_name'], "to", result.group(1), "using line \n\t<", lines[index], ">")
            r['symbol_val'] = result.group(1)
            continue

def parse_elf_section_headers(lines):
    sections = {}
    for i, l in enumerate(lines):
        if l == "Section Headers:":
            break

    for l in lines[i+3:]: # Ignore title line, column headers
        if l == "Key to Flags:":
            break # no more section headers
        if "NULL" in l:
            continue # don't want NULL header type

        items = l[5:].split() # strip section numbers at the head of each line
        if len(items) == 10:
            sections[items[0]] = {
                'type': items[1],
                'addr': items[2],
                'off': items[3],
                'size': items[4],
                'es': items[5],
                'flg': items[6],
                'lk': items[7],
                'inf': items[8],
                'al': items[9],
            }
        elif len(items) == 9: # same thing, just missing a Flg field
            sections[items[0]] = {
                'type': items[1],
                'addr': items[2],
                'off': items[3],
                'size': items[4],
                'es': items[5],
                'lk': items[6],
                'inf': items[7],
                'al': items[8],
            }
        else:
            print('[Warning] expected either 9 or 10 section header fields, got', len(items))
            continue
        

    return sections


def resolve_got_symbols(sections, relocations, input_elf_content):

    for r in relocations:
        # ELF section offset + offset within GOT
        got_entry_offset = int(sections['.got']['off'], base=16) + r['got_offset']
        # Make sure function calls in Thumb mode!
        thumb_symbol_val = int(r['symbol_val'], base=16) | 1
        # Convert resolved address (updated symbol_val) to little endian before writing
        value = thumb_symbol_val.to_bytes(4, byteorder='little')
        input_elf_content[got_entry_offset:got_entry_offset+4] = value

    return input_elf_content


def patch_ptr_indirection(elf_sections, relocations, dump_lines, elf_content):

    # Search .text section dump for `ldr` instructions annotated with relocation offset
    #     --> destination register: r_gotoff
    for i, l in enumerate(dump_lines):
        if l == "Disassembly of section .text:":
            break

    patterns = []
    for r in relocations:
        patterns.append(re.compile('.*ldr\s*(\S\S),\s*\[pc,.*\('+r['offset']+'.*\)'))
    
    for j, l in enumerate(dump_lines[i+1:]): # skip title line
        if l.startswith("Disassembly of section"):
            break
        for p in patterns:
            result = p.match(l)
            if result:
                r_gotoff = result.group(1)

                # Find the next `ldr` instruction indexing into GOT using r_gotoff
                #     --> destination for GOT entry: r_fptr
                gotoff_ldr_pattern = re.compile('.*ldr(\.w)?\s*(\S\S),\s*\[r9,\s*'+r_gotoff+'\]')
                for k, l2 in enumerate(dump_lines[i+1+j+1:]):
                    if l2.startswith("Disassembly of section"):
                        break
                    result = gotoff_ldr_pattern.match(l2)
                    if result:
                        r_fptr = result.group(2)

                        # Replace the _next_ `ldr` instruction indexed using r_fptr with a direct `mov`;
                        # this is the pointer indirection we no longer need since we manually
                        # inserted the function location into the GOT
                        indirection_pattern = re.compile('([0-9a-f]{8}).*ldr\s*(\S\S),\s*\['+r_fptr+',\s*#0\]')
                        for l3index, l3 in enumerate(dump_lines[i+1+j+1+k+1:]): # may god have mercy on my soul
                            if l3.startswith("Disassembly of section"):
                                break
                            result = indirection_pattern.match(l3)
                            if result:
                                instruction_addr = result.group(1)
                                r_dest = result.group(2)
                                text_section_offset = \
                                    int(instruction_addr, base=16) - int(elf_sections['.text']['addr'], base=16)
                                elf_content_offset = \
                                    int(elf_sections['.text']['off'], base=16) + text_section_offset

                                mov_instr = (0b010001100 << 7) | ((int(r_fptr[1]) & 0b1111) << 3) | (int(r_dest[1]) & 0b111)
                                mov_val = (mov_instr).to_bytes(2, byteorder='little')
                                elf_content[elf_content_offset:elf_content_offset+2] = mov_val
                        break
        
    return elf_content

# For resolved relocations, change dynamic relocation type from R_ARM_RELATIVE to R_ARM_NONE
# The format of the .rel.dyn section is a simple list of 8-byte entries, so we manipulate it directly:
#  ([32 bit offset into GOT] [32 bit info field])*
def patch_dyn_relocations(elf_sections, relocations, elf_content):

    if '.rel.dyn' not in elf_sections:
        return elf_content

    dyn_section = elf_sections['.rel.dyn']
    dyn_elf_base = int(dyn_section['off'], base=16)

    for dyn_offset in range(0, int(dyn_section['size'], base=16), 8):
        got_offset_val = int.from_bytes(elf_content[dyn_elf_base+dyn_offset:dyn_elf_base+dyn_offset+4], byteorder='little')
        for r in relocations:
            if r['got_offset'] == got_offset_val: # set R_ARM_NONE = 0
                arm_none_val = (0x0).to_bytes(4, byteorder='little')
                elf_content[dyn_elf_base+dyn_offset+4:dyn_elf_base+dyn_offset+8] = arm_none_val

    return elf_content 


def patch_pic_entry(elf_sections, dump_lines, elf_content):
    if not dump_lines:
        return elf_content

    pattern = re.compile("([0-9a-f]{8})\s*<main>:")
    result = find_symbol(dump_lines, pattern)
    if result:
        hdr_offset = int(elf_sections['.pic_hdr']['off'], base=16)
        val = (int(result.group(1), base=16)).to_bytes(4, byteorder='little')
        elf_content[hdr_offset:hdr_offset+4] = val
    else:
        print('[Warning] could not patch PIC entry, .text.main not found')

    return elf_content
    

def relocate(elf_in, elf_out, extern_map, secure_map, dump, readelf):

    if extern_map:
        map_lines = extract_lines(extern_map)
    else:
        map_lines = None

    if secure_map:
        secure_map_lines = extract_lines(secure_map)
    else:
        secure_map_lines = None
    
    status = subprocess.call("arm-none-eabi-objdump -D " + elf_in.name + " > " + dump.name, shell=True)
    if status != 0:
        print("[Error] arm-none-eabi-objdump returned status", status)
        return
    dump_lines = extract_lines(dump)

    status = subprocess.call("arm-none-eabi-readelf -a " + elf_in.name + " > " + readelf.name, shell=True)
    if status != 0:
        print("[Error] arm-none-eabi-readelf returned status" , status)
        return
    readelf_lines = extract_lines(readelf)

    # Extract info from .rel.text section
    text_relocations = parse_rel_text(readelf_lines, ignore_nonzero_symbol_vals=True)
    for r in text_relocations:
        if r['type'] != 'R_ARM_GOT_BREL':
            print("[Warning] found relocation with type", r['type'], "--", r)

    # Associate relocations with GOT offsets
    parse_got_offsets(dump_lines, text_relocations)
    # Extract locations of unresolved functions
    parse_symbol_locations(map_lines, text_relocations)
    parse_symbol_locations(secure_map_lines, text_relocations)
     
    input_content = bytearray(elf_in.read())

    # Extract ELF header information for section offsets
    elf_sections = parse_elf_section_headers(readelf_lines)
    # Modify GOT affected by text relocations to resolve external functions
    elf_content = resolve_got_symbols(elf_sections, text_relocations, input_content)

    # Patch pointer indirection -- function addresses are directly in GOT, so
    # we replace the second indirection with a NOP
    # Note: this assumes that extern functions are being declared as fn
    # pointers, as a workaround to lack of `fno-plt` support in the
    # arm-eabi-none toolchain
    elf_content = patch_ptr_indirection(elf_sections, text_relocations, dump_lines, elf_content)

    # Since we manually resolved the external symbols into the GOT, remove the
    # dynamic relocation entries for the same indices in .rel.dyn
    elf_content = patch_dyn_relocations(elf_sections, text_relocations, elf_content)

    # Patch the entry function in the PIC header to point to the right symbol
    #    note: not currently necessary, given direct placement of main() at top of .text section
    #elf_content = patch_pic_entry(elf_sections, dump_lines, elf_content)
    
    # Finally, write modified ELF back to file
    elf_out.write(elf_content)


parser = argparse.ArgumentParser(description='Relocates unresolved extern function calls in ELF binaries')
parser.add_argument('-o', '--output', help='Output path', default='relocated.elf')
parser.add_argument('input', help='ELF file with unresolved extern calls to relocate')
parser.add_argument('map', help='Map file with extern symbols')
parser.add_argument('securemap', help='Map file with secure symbols')
args = parser.parse_args()

# Zephyr map not linked the first time compiled
# TODO: fix this
if not os.path.exists(args.map):
    print("[Warning] no Zephyr map file at " + args.map + ", rebuild necessary")
    extern_map = None
else:
    extern_map = open(args.map, 'r')

if not os.path.exists(args.securemap):
    print("[Warning] no secure map file at " + args.securemap)
    secure_map = None
else:
    secure_map = open(args.securemap, 'r')

# Generate objdump and readelf outputs for easy analysis
with tempfile.NamedTemporaryFile(mode='w+') as temp_dump, tempfile.NamedTemporaryFile(mode='w+') as temp_readelf, \
        open(args.output, 'w+b') as output_elf, open(args.input, 'rb') as input_elf:
    
    if secure_map:
        relocate(input_elf, output_elf, extern_map, secure_map, temp_dump, temp_readelf)
    else:
        status = subprocess.call("cp " + args.input + " " + args.output, shell=True)

if extern_map:
    extern_map.close()

print("Relocated PIC app")

