import argparse
import os
from elftools.elf.elffile import ELFFile
from elftools.elf import enums, descriptions
import re
import subprocess
import sys
import tempfile

MUSCA_DATA_BASE = 0x20000000

'''
Read file contents into whitespace-stripped line list.
'''
def read_content(f):
    if not f:
        return None
    return [l.strip() for l in f.readlines()]


'''
Helper to print warnings.
'''
ENABLE_WARNING = True
def print_warning(s):
    if ENABLE_WARNING:
        print('[Warning] (relocate.py): ' + s)


'''
Info helper.
'''
ENABLE_INFO = True
def print_info(s):
    if ENABLE_INFO:
        print('[Info]    (relocate.py): ' + s)


'''
A debug helper, why not?
'''
ENABLE_DEBUG = True
def print_debug(s):
    if ENABLE_DEBUG:
        print('[Debug]   (relocate.py): ' + s)


'''
And an error printer :)
'''
ENABLE_ERROR = True
def print_error(s):
    if ENABLE_ERROR:
        print('[Error]   (relocate.py): ' + s)


def write_to_elf(elf_bytes, offset, val, n, little_endian=True):
    byte_val = val.to_bytes(n, byteorder='little' if little_endian else 'big')
    elf_bytes[offset:offset+n] = byte_val


'''
Parse .rel.text entries, ignoring relocations that are nonzero (matching
entries in .rel.dyn for dynamic relocation) if desired. Zero-valued relocations
are more likely missing extern references that must be resolved.
'''
def parse_text_relocations(e, ignore_nonzero_symbol_vals=False):

    rel_text = e.get_section_by_name('.rel.text')
    rel_symtab = e.get_section(rel_text['sh_link'])
    rel_stringtable = e.get_section_by_name('.strtab')

    parsed_relocations = []

    for relocation in rel_text.iter_relocations():
        relocation_symbol = rel_symtab.get_symbol(relocation['r_info_sym'])

        if ignore_nonzero_symbol_vals and relocation_symbol['st_value'] != 0:
            continue

        relocation_type = descriptions._DESCR_RELOC_TYPE_ARM[relocation['r_info_type']]
        if relocation_type != 'R_ARM_GOT_BREL':
            print_warning('found .text relocation with unexpected type ' + relocation_type)

        parsed_relocations.append({
            'offset': '{:08x}'.format(relocation['r_offset']),
            'v2_offset': relocation['r_offset'],
            'info': '{:08x}'.format(relocation['r_info']),
            'type': relocation_type,
            'symbol_val': '{:08x}'.format(relocation_symbol['st_value']),
            'symbol_name': rel_stringtable.get_string(relocation_symbol['st_name']),
        })

    return parsed_relocations


def parse_got_offsets(e, relocations):
    # We're looking in the text section for the GOT offsets matching the parsed relocations
    text = e.get_section_by_name('.text')
    for r in relocations:
        text_offset = relocations[0]['v2_offset'] - text['sh_addr']
        
        if text_offset < 0 or text_offset >= text['sh_size']:
            print_warning('Relocation .text offset has bad value ' + text_offset)
            continue

        r['got_offset'] = text.data()[text_offset]
    
    
'''
Search through the provided `.map` file contents to find symbol addresses
matching the outstanding relocation entries.
'''
def parse_symbol_locations(map_contents, relocations):
    if not map_contents: # don't do anything if the map file doesn't exist
        return

    # Patterns can be very finnicky depending on the system and format of the
    # map file Should return a list of pattern lists, for which the last
    # result.group(1) contains the correct symbol location when applied to
    # successive lines after the first successful match.
    def generate_patterns(relocation):
        
        return [
            [ # symbol address on one line
                re.compile('\.text\S*\.?'+relocation['symbol_name']+'\s*0x0*([0-9a-f]{8})')
            ],
            [ # symbol address wrapped to next line
                re.compile('\.text\S*\.?'+relocation['symbol_name']+'\Z'),
                re.compile('\s*0x0*([0-9a-f]{8})')
            ],
            [ # use the symbol name after the address instead, since apparently sometimes that's necessary
                re.compile('\s*0x0*([0-9a-f]{8})\s*'+r['symbol_name']+'\Z')
            ],
        ]

    def apply_pattern(map_contents, pattern_list):
        base_index = 0 
        final_result = None 

        for i, l in enumerate(map_contents):
            result = pattern_list[0].match(l)
            if result:
                base_index = i
                if len(pattern_list) == 1: # last pattern (only one)
                    final_result = result
                break
                
        for i, p in enumerate(pattern_list[1:]):
            result = p.match(map_contents[base_index + i])
            if not result:
                break

            if i == len(pattern_list) - 1: # last pattern
                final_result = result

        return base_index, final_result

    for r in relocations:
        patterns = generate_patterns(r)
        for p_list in patterns:
            line_index, result = apply_pattern(map_contents, p_list)
            if result:
                r['symbol_val'] = result.group(1)

                # Print the symbol resolution for sanity checking
                info_string = 'resolved symbol `' + r['symbol_name'] + '` to ' + r['symbol_val'] + ' using line(s) \n'
                for i in range(len(p_list)):
                    info_string += '\t\t' + map_contents[line_index + i]
                print_debug(info_string)
                break


def resolve_got_symbols(e, e_contents, relocations):

    got = e.get_section_by_name('.got')
    for r in relocations:
        # Convert function calls to Thumb mode
        symbol_val = int(r['symbol_val'], base=16)
        if symbol_val < MUSCA_DATA_BASE:
            symbol_val |= 1

        got_entry_offset = got['sh_offset'] + r['got_offset']
        write_to_elf(e_contents, got_entry_offset, symbol_val, 4)


def patch_ptr_indirection(e, e_contents, relocations, dump_contents):

    start_idx, end_idx = None, None
    for i, l in enumerate(dump_contents):
        if l == 'Disassembly of section .text:':
            start_idx = i
        elif start_idx and l.startswith('Disassembly of section'):
            end_idx = i
            break

    text_contents = dump_contents[start_idx:end_idx]

    # Search .text section dump for `ldr` instructions annotated with relocation offset
    #     --> destination register: r_gotoff
    patterns = []
    for r in relocations:
        patterns.append(re.compile(
            # look for `ldr` instructions that index off the PC to access a
            # relocation offset into the text segment. Since loads are
            # PC-relative, we're relying on the `objdump` commented annotations
            # to identify which loads correspond to the correct absolute
            # offsets.
            '.*ldr\s*(\S\S),\s*\[pc,.*\('+r['offset']+'.*\)'
        ))

    # Important note: assumes that the `instruction_addr` points to a 16-bit instruction
    def overwrite_with_mov(instruction_addr, r_src, r_dst):

        text = e.get_section_by_name('.text')
        text_offset = int(instruction_addr, base=16) - text['sh_addr']
        offset_into_elf_contents = text['sh_offset'] + text_offset

        mov_instr = (0b010001100 << 7) | ((int(r_src[1:]) & 0b1111) << 3) | (int(r_dst[1:]) & 0b111)
        write_to_elf(e_contents, offset_into_elf_contents, mov_instr, 2)

    # Given a register containing a GOT offset, patch the following lines to
    # remove the additional function pointer indirection.
    #    (1) find the next `ldr` instruction indexing into GOT (r9) using
    #        `r_gotoff`, storing the GOT entry into `r_fptr`
    #    (2) replace the _next_ `ldr` instruction indexed using r_fptr with a
    #        direct `mov`; this is the pointer indirection we no longer need
    #        since we manually inserted the function location into the GOT
    def patch_indirection(lines, r_gotoff):

        # (1)
        r_fptr = None
        fptr_load_pattern = re.compile('.*ldr(\.w)?\s*(\S\S),\s*\[r9,\s*'+r_gotoff+'\]')
        for i, l in enumerate(lines):
            result = fptr_load_pattern.match(l)
            if result:
                r_fptr = result.group(2)        
                indirection_lines = lines[i+1:]
                break

        if not r_fptr:
            print_warning('`r_fptr` not found while patching GOT function pointer indirection')
            return

        # (2) 
        indirection_load_pattern = re.compile('([0-9a-f]{8}).*ldr\s*(\S\S),\s*\['+r_fptr+'(,\s*#0)?\]')
        for i, l in enumerate(indirection_lines):
            result = indirection_load_pattern.match(l)
            if result:
                instruction_addr = result.group(1)
                r_dest = result.group(2)
                overwrite_with_mov(instruction_addr, r_fptr, r_dest)

    for i, l in enumerate(text_contents):
        for p in patterns:
            result = p.match(l)
            if result:
                patch_indirection(text_contents[i+1:], result.group(1))


# For resolved relocations, change dynamic relocation type from R_ARM_RELATIVE
# to R_ARM_NONE The format of the .rel.dyn section is a simple list of 8-byte
# entries: ([32 bit offset into GOT] [32 bit info field])
def patch_dyn_relocations(e, e_contents, relocations):

    rel_dyn = e.get_section_by_name('.rel.dyn')
    rel_dyn_base_offset = rel_dyn['sh_offset']

    for i in range(rel_dyn.num_relocations()):
        relocation_entry = rel_dyn.get_relocation(i)
        for r in relocations:
            if r['got_offset'] == relocation_entry['r_offset']:
                write_to_elf(e_contents, rel_dyn_base_offset + (i * 8), enums.ENUM_RELOC_TYPE_ARM['R_ARM_NONE'], 4)


# Patch the entry function in the PIC header to point to the right symbol
def patch_pic_entry(e, e_contents, dump_contents):

    pic_hdr = e.get_section_by_name('.pic_hdr')

    # Look for main
    pattern = re.compile('([0-9a-f]{8})\s*<main>:')
    for i, l in enumerate(dump_contents):
        result = pattern.match(l)
        if result:
            write_to_elf(e_contents, pic_hdr['sh_offset'], int(result.group(1), base=16), 4)


def relocate(elf_in_file, elf_out_file, extern_map, secure_map, dump, readelf):

    elf_in = ELFFile(elf_in_file)

    extern_map_contents = read_content(extern_map)
    secure_map_contents = read_content(secure_map)

    status = subprocess.call('arm-none-eabi-objdump -D ' + elf_in_file.name + ' > ' + dump.name, shell=True)
    if status != 0:
        print_error('arm-none-eabi-objdump returned status ' + status)
        return
    dump_contents = read_content(dump)

    # Extract info from .rel.text section
    text_relocations = parse_text_relocations(elf_in, ignore_nonzero_symbol_vals=True)

    # Associate relocations with GOT offsets
    parse_got_offsets(elf_in, text_relocations)
    # Extract locations of unresolved functions
    parse_symbol_locations(extern_map_contents, text_relocations)
    parse_symbol_locations(secure_map_contents, text_relocations)
     
    elf_contents = bytearray(elf_in_file.read())

    # Modify GOT affected by text relocations to resolve external functions
    resolve_got_symbols(elf_in, elf_contents, text_relocations)

    # Patch pointer indirection -- function addresses are directly in GOT, so
    # we replace the second indirection with a NOP
    # Note: this assumes that extern functions are being declared as fn
    # pointers, as a workaround to lack of `fno-plt` support in the
    # arm-eabi-none toolchain
    patch_ptr_indirection(elf_in, elf_contents, text_relocations, dump_contents)

    # Since we manually resolved the external symbols into the GOT, remove the
    # dynamic relocation entries for the same indices in .rel.dyn
    patch_dyn_relocations(elf_in, elf_contents, text_relocations)

    # Not currently necessary, given direct placement of main() at top of .text section
    #patch_pic_entry(elf_in, elf_contents, dump_contents)
    
    # Finally, write modified ELF back to file
    elf_out_file.write(elf_contents)


parser = argparse.ArgumentParser(description='Relocates unresolved extern function calls in ELF binaries')
parser.add_argument('-o', '--output', help='Output path', default='relocated.elf')
parser.add_argument('input', help='ELF file with unresolved extern calls to relocate')
parser.add_argument('map', help='Map file with extern symbols')
parser.add_argument('securemap', help='Map file with secure symbols')
args = parser.parse_args()

if not os.path.exists(args.map):
    print_warning('no Zephyr map file at ' + args.map + ', no relocations for external symbols processed.')
    extern_map = None
else:
    extern_map = open(args.map, 'r')

if not os.path.exists(args.securemap):
    print_warning('no secure map file at ' + args.securemap + ', no relocations for TZ secure veneers processed.')
    secure_map = None
else:
    secure_map = open(args.securemap, 'r')

# Generate objdump and readelf outputs for easy analysis
with tempfile.NamedTemporaryFile(mode='w+') as temp_dump, \
     tempfile.NamedTemporaryFile(mode='w+') as temp_readelf, \
     open(args.output, 'w+b') as output_elf, \
     open(args.input, 'rb') as input_elf:
    
    # TODO: just output original elf if no extern map
    #    status = subprocess.call("cp " + args.input + " " + args.output, shell=True)
    relocate(input_elf, output_elf, extern_map, secure_map, temp_dump, temp_readelf)

if extern_map:
    extern_map.close()

if secure_map:
    secure_map.close()

print_info('relocated PIC app saved as ' + args.output)

