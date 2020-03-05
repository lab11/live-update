import argparse
from capstone import *
from capstone.arm import *
from elftools.elf.elffile import ELFFile
from elftools.elf import enums, descriptions
import os
import re
import subprocess
import sys
import tempfile

MUSCA_DATA_BASE = 0x20000000

ARM_REG_VALS = {
    'r0': 0,
    'r1': 1,
    'r2': 2,
    'r3': 3,
    'r4': 4,
    'r5': 5,
    'r6': 6,
    'r7': 7,
    'r8': 8,
    'r9': 9,
    'r10': 10,
    'r11': 11,
    'r12': 12,
    'sb': 9,
    'sl': 10,
    'fp': 11,
    'ip': 12,
}

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
def print_warning(s, *argv):
    if ENABLE_WARNING:
        out = '[Warning] (relocate.py): ' + str(s)
        for a in argv:
            out += ' ' + str(a)
        print(out)


'''
Info helper.
'''
ENABLE_INFO = True
def print_info(s, *argv):
    if ENABLE_INFO:
        out = '[Info]    (relocate.py): ' + str(s)
        for a in argv:
            out += ' ' + str(a)
        print(out)


'''
A debug helper, why not?
'''
ENABLE_DEBUG = True
def print_debug(s, *argv):
    if ENABLE_DEBUG:
        out = '[Debug]   (relocate.py): ' + str(s)
        for a in argv:
            out += ' ' + str(a)
        print(out)


'''
And an error printer :)
'''
ENABLE_ERROR = True
def print_error(s, *argv):
    if ENABLE_ERROR:
        out = '[Error]   (relocate.py): ' + str(s)
        for a in argv:
            out += ' ' + str(a)
        print(out)


def write_to_elf(elf_bytes, offset, val, n, little_endian=True):
    byte_val = val.to_bytes(n, byteorder='little' if little_endian else 'big')
    elf_bytes[offset:offset+n] = byte_val


'''
Parse .rel.text entries, ignoring relocations that are nonzero (matching
entries in .rel.dyn for dynamic relocation) if desired. Zero-valued relocations
are more likely missing extern references that must be resolved.
'''
def parse_text_relocations(e, ignore_nonzero_symbol_vals=False):

    parsed_relocations = []

    rel_text = e.get_section_by_name('.rel.text')
    if not rel_text:
        return parsed_relocations

    rel_symtab = e.get_section(rel_text['sh_link'])
    rel_stringtable = e.get_section_by_name('.strtab')

    for relocation in rel_text.iter_relocations():
        relocation_symbol = rel_symtab.get_symbol(relocation['r_info_sym'])

        if ignore_nonzero_symbol_vals and relocation_symbol['st_value'] != 0:
            continue

        relocation_type = descriptions._DESCR_RELOC_TYPE_ARM[relocation['r_info_type']]
        if relocation_type != 'R_ARM_GOT_BREL':
            print_warning('found .text relocation with unexpected type ' + relocation_type)

        parsed_relocations.append({
            'offset': '{:08x}'.format(relocation['r_offset']),
            'offset_val': relocation['r_offset'],
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
        text_offset = r['offset_val'] - text['sh_addr']
        
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
            [ # symbol address on one line, try first for exact .text.`fn` match
                re.compile('\.text\.'+relocation['symbol_name']+'\s*0x0*([0-9a-f]{8})')
            ],
            [ # symbol address on one line
                re.compile('\.text\S*'+relocation['symbol_name']+'\s*0x0*([0-9a-f]{8})')
            ],
            [ # symbol address wrapped to next line, exact match first
                re.compile('\.text\.?'+relocation['symbol_name']),
                re.compile('\s*0x0*([0-9a-f]{8})')
            ],
            [ # symbol address wrapped to next line
                re.compile('\.text\S*?'+relocation['symbol_name']),
                re.compile('\s*0x0*([0-9a-f]{8})')
            ],
            [ # use the symbol name after the address instead, since apparently sometimes that's necessary
                re.compile('\s*0x0*([0-9a-f]{8})\s*'+r['symbol_name'])
            ],
        ]

    def apply_pattern(map_contents, pattern_list):
        print_debug('apply_pattern(contents, ' + str(pattern_list) + ')')

        final_result = None 
        first_match_index = -1

        print_debug('\ttrying to find first line match for pattern ' + str(pattern_list[0]) + ' in map contents')
        for i, l in enumerate(map_contents):
            result = pattern_list[0].match(l)
            if result:
                print_debug('\tfirst line matched at index ' + str(i) + ' line: ' + l)
                if len(pattern_list) == 1: # last pattern (only one)
                    print_debug('\tonly one pattern, returning final result')
                    final_result = result
                else:
                    print_debug('\tmore than one pattern, setting first match index to ' + str(i))
                first_match_index = i
                break
                
        if first_match_index >= 0:
            print_debug('\tfirst_match_index set to 0 or greater, proceeding to secondary patterns')
            for i, p in enumerate(pattern_list[1:]):
                print_debug('\ttrying to find ' + str(i+1) + 'line match for pattern ' + str(p) + ' in text line ' + map_contents[first_match_index + 1 + i])
                result = p.match(map_contents[first_match_index + 1 + i])
                if not result:
                    print_debug("\tdidn't find a match")
                    break

                print_debug('\t'+str(i+1)+' line matched')
                if i == len(pattern_list[1:]) - 1: # last pattern
                    print_debug("\tthat was the last pattern, returning final result")
                    final_result = result

        return first_match_index, final_result

    for r in relocations:
        print_debug('generating patterns for unresolved symbol `' + r['symbol_name'] + '`')
        patterns = generate_patterns(r)
        for p_list in patterns:
            line_index, result = apply_pattern(map_contents, p_list)
            if result:
                r['symbol_val'] = result.group(1)

                # Print the symbol resolution for sanity checking
                info_string = 'resolved symbol `' + r['symbol_name'] + '` (GOT offset: ' + hex(r['got_offset']) + ') to ' + r['symbol_val'] + ' using line(s) \n'
                for i in range(len(p_list)):
                    info_string += '\t\t' + map_contents[line_index + i]
                print_info(info_string)
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

def to_hex(s, prefix_0x=True):
    if prefix_0x:
        return " ".join("0x{0:02x}".format(c) for c in s)
    else:
        return " ".join("{0:02x}".format(c) for c in s)

def to_x_32(s):
    from struct import pack
    if not s: return '0'
    x = pack(">i", s)
    while x[0] in ('\0', 0): x = x[1:]
    
    r = "".join("{0:02x}".format(c) for c in x)
    while r[0] == '0': r = r[1:]
    
    return r

def print_insn_detail(insn):
    # print address, mnemonic and operands
    print_debug("0x%x:\t%s\t%s" % (insn.address, insn.mnemonic, insn.op_str))

    # "data" instruction generated by SKIPDATA option has no detail
    if insn.id == 0:
        return

    if len(insn.operands) > 0:
        print_debug("\top_count: %u" % len(insn.operands))
        c = 0
        for i in insn.operands:
            if i.type == ARM_OP_REG:
                print_debug("\t\toperands[%u].type: REG = %s" % (c, insn.reg_name(i.reg)))
            if i.type == ARM_OP_IMM:
                print_debug("\t\toperands[%u].type: IMM = 0x%s" % (c, to_x_32(i.imm)))
            if i.type == ARM_OP_PIMM:
                print_debug("\t\toperands[%u].type: P-IMM = %u" % (c, i.imm))
            if i.type == ARM_OP_CIMM:
                print_debug("\t\toperands[%u].type: C-IMM = %u" % (c, i.imm))
            if i.type == ARM_OP_FP:
                print_debug("\t\toperands[%u].type: FP = %f" % (c, i.fp))
            if i.type == ARM_OP_SYSREG:
                print_debug("\t\toperands[%u].type: SYSREG = %u" % (c, i.reg))
            if i.type == ARM_OP_SETEND:
                if i.setend == ARM_SETEND_BE:
                    print_debug("\t\toperands[%u].type: SETEND = be" % c)
                else:
                    print_debug("\t\toperands[%u].type: SETEND = le" % c)
            if i.type == ARM_OP_MEM:
                print_debug("\t\toperands[%u].type: MEM" % c)
                if i.mem.base != 0:
                    print_debug("\t\t\toperands[%u].mem.base: REG = %s" \
                        % (c, insn.reg_name(i.mem.base)))
                if i.mem.index != 0:
                    print_debug("\t\t\toperands[%u].mem.index: REG = %s" \
                        % (c, insn.reg_name(i.mem.index)))
                if i.mem.scale != 1:
                    print_debug("\t\t\toperands[%u].mem.scale: %u" \
                        % (c, i.mem.scale))
                if i.mem.disp != 0:
                    print_debug("\t\t\toperands[%u].mem.disp: 0x%s" \
                        % (c, to_x_32(i.mem.disp)))
                if i.mem.lshift != 0:
                    print_debug("\t\t\toperands[%u].mem.lshift: 0x%s" \
                        % (c, to_x_32(i.mem.lshift)))

            if i.neon_lane != -1:
                print_debug("\t\toperands[%u].neon_lane = %u" % (c, i.neon_lane))

            if i.access == CS_AC_READ:
                print_debug("\t\toperands[%u].access: READ\n" % (c))
            elif i.access == CS_AC_WRITE:
                print_debug("\t\toperands[%u].access: WRITE\n" % (c))
            elif i.access == CS_AC_READ | CS_AC_WRITE:
                print_debug("\t\toperands[%u].access: READ | WRITE\n" % (c))

            if i.shift.type != ARM_SFT_INVALID and i.shift.value:
                print_debug("\t\t\tShift: %u = %u" \
                    % (i.shift.type, i.shift.value))
            if i.vector_index != -1:
                print_debug("\t\t\toperands[%u].vector_index = %u" %(c, i.vector_index))
            if i.subtracted:
                print_debug("\t\t\toperands[%u].subtracted = True" %c)

            c += 1

    if insn.update_flags:
        print_debug("\tUpdate-flags: True")
    if insn.writeback:
        print_debug("\tWrite-back: True")
    if not insn.cc in [ARM_CC_AL, ARM_CC_INVALID]:
        print_debug("\tCode condition: %u" % insn.cc)
    if insn.cps_mode:
        print_debug("\tCPSI-mode: %u" %(insn.cps_mode))
    if insn.cps_flag:
        print_debug("\tCPSI-flag: %u" %(insn.cps_flag))
    if insn.vector_data:
        print_debug("\tVector-data: %u" %(insn.vector_data))
    if insn.vector_size:
        print_debug("\tVector-size: %u" %(insn.vector_size))
    if insn.usermode:
        print_debug("\tUser-mode: True")
    if insn.mem_barrier:
        print_debug("\tMemory-barrier: %u" %(insn.mem_barrier))

    (regs_read, regs_write) = insn.regs_access()

    if len(regs_read) > 0:
        print_debug("\tRegisters read:")
        for r in regs_read:
            print_debug(" %s" %(insn.reg_name(r)))
        print_debug("")

    if len(regs_write) > 0:
        print_debug("\tRegisters modified:")
        for r in regs_write:
            print_debug(" %s" %(insn.reg_name(r)))
        print_debug("")


def patch_ptr_indirection(e, e_contents, relocations, dump_contents):

    text_section = e.get_section_by_name('.text')
    offset = text_section['sh_offset']
    size = text_section['sh_size']

    text_bytes = e_contents[offset:offset+size]
    mode = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_LITTLE_ENDIAN + CS_MODE_V8)
    mode.detail = True

    def get_got_load_register(instr_list, idx):

        instr = instr_list[idx]
        if not instr.mnemonic.startswith("ldr"):
            #print_debug('failed ldr', instr.mnemonic)
            return None

        o = instr.operands[1]
        if o.type != ARM_OP_MEM or (instr.reg_name(o.mem.base) != 'sb'):
            #print_debug('failed op mem', o.type, instr.reg_name(o.mem.base) != 'sb')
            return None

        dest_o = instr.operands[0]
        if dest_o.type != ARM_OP_REG:
            #print_debug('failed dst op reg', dest_o.type)
            return None

        return_reg = instr.reg_name(dest_o.reg)

        for curr_instr in reversed(instr_list[:idx]):
            if curr_instr.mnemonic.startswith('ldr') and curr_instr.reg_name(curr_instr.operands[0].reg) == instr.reg_name(o.mem.index) \
                    and curr_instr.reg_name(curr_instr.operands[1].mem.base) == 'pc':
                got_index_location = curr_instr.operands[1].mem.disp + curr_instr.address + 2
                if curr_instr.address % 4 == 0:
                    got_index_location += 2

                matched_r_flag = False
                for r in relocations:
                    if r['offset_val'] == got_index_location:
                        matched_r_flag = True

                if not matched_r_flag: # couldn't find a text relocation, so it must be a data reference (keep indirections)
                    return_reg = None
                break

        return return_reg

    def get_branch_dest_idx(instr_list, target_instr_addr):
        for idx, i in enumerate(instr_list):
            if i.address == target_instr_addr:
                return idx

    def destroy_indirection(instr):

        text_offset = instr.address - text_section['sh_addr']
        offset_into_elf_contents = text_section['sh_offset'] + text_offset

        print_insn_detail(instr)
        
        r_dst = instr.reg_name(instr.operands[0].reg)
        if r_dst not in ARM_REG_VALS:
            print_warning('indirection destination register', r_dst, 'not found in ARM_REG_VALS')
            return
        r_dst_val = ARM_REG_VALS[r_dst]

        r_src = instr.reg_name(instr.operands[1].mem.base)
        if r_src not in ARM_REG_VALS:
            print_warning('indirection source register', r_src, 'not found in ARM_REG_VALS')
            return
        r_src_val = ARM_REG_VALS[r_src]

        mov_instr = (0b01000110 << 8) | ((r_dst_val >> 3) & 0b1) << 7 | ((r_src_val & 0b1111) << 3) | (r_dst_val & 0b111)
        write_to_elf(e_contents, offset_into_elf_contents, mov_instr, 2)

        if instr.size == 4: # append nop
            write_to_elf(e_contents, offset_into_elf_contents+2, 0x0, 2)

    def search_and_destroy_indirections(instr_list, idx, got_val_reg):
        visited_instructions = [idx]
        pending_search_queue = [idx + 1] # we already saw the GOT load, let's start at the next line

        while len(pending_search_queue) > 0:
            curr_idx = pending_search_queue.pop(0) 
            print_debug('\tdequeueing instruction: idx', curr_idx, 'addr', hex(instr_list[curr_idx].address))
            
            # iterate through instructions looking for indirect loads from `got_val_reg`
            # until we leave the local scope (return from function) or register is overwritten

            while True:
                if curr_idx >= len(instr_list):
                    print_debug('\tran out of instructions, stopping iteration')
                    break # no sense in reading off the end of .text

                curr_instr = instr_list[curr_idx]
                print_debug('\t** index, address, mnemonic:', curr_idx, hex(curr_instr.address), curr_instr.mnemonic)
                if curr_idx in visited_instructions:
                    print_debug('\tgoing in a loop, terminating path')
                    #print('>>>>')
                    #print_insn_detail(curr_instr) 
                    #print('<<<<')
                    break # going in a loop, stop while we still can

                visited_instructions.append(curr_idx)

                if curr_instr.mnemonic.startswith('ldr'): # check for bad function pointer dereference
                    o1 = curr_instr.operands[1]
                    if o1.type == ARM_OP_MEM and curr_instr.reg_name(o1.mem.base) == got_val_reg:
                        print_debug('\tfound bad function pointer dereference')
                        #print('>>>>')
                        #print_insn_detail(curr_instr) 
                        #print('<<<<')

                        dest_reg = curr_instr.reg_name(curr_instr.operands[0].reg)
                        print_debug('\tdestination register', dest_reg)
                        destroy_indirection(curr_instr)

                if curr_instr.mnemonic.startswith('bx') or curr_instr.mnemonic.startswith('bxns'):
                    print_debug('\tbx or bxns, terminating path')
                    #print('>>>>')
                    #print_insn_detail(curr_instr) 
                    #print('<<<<')
                    break # returning from function, no need to continue

                if curr_instr.mnemonic.startswith('pop'):
                    regs_read, regs_write = curr_instr.regs_access()
                    pop_flag = False
                    for r in regs_write:
                        if curr_instr.reg_name(r) == 'pc':
                            print_debug('\tpop pc return, terminating path')
                            #print('>>>>')
                            #print_insn_detail(curr_instr) 
                            #print('<<<<')
                            pop_flag = True
                            break

                    if pop_flag:
                        break # common to return from function by loading old PC off stack

                CONDITIONAL_BRANCH_LIST = ['beq', 'bne', 'ble', 'blt', 'bge', 'bgt', 'cbnz', 'cbz']
                cb_flag = False
                for b in CONDITIONAL_BRANCH_LIST:
                    if curr_instr.mnemonic.startswith(b):
                        if curr_instr.operands[0].type == ARM_OP_IMM:
                            target_o = curr_instr.operands[0]
                        elif curr_instr.operands[1].type == ARM_OP_IMM:
                            target_o = curr_instr.operands[1]
                        else:
                            target_o = None

                        if target_o:
                            cb_flag = True
                            target = get_branch_dest_idx(instr_list, target_o.imm)
                            print_debug('\tconditional jump detected, appending index', target)
                            #print('>>>>')
                            #print_insn_detail(curr_instr)
                            #print('<<<<')

                            pending_search_queue.append(target) # queue passing condition 
                            curr_idx += 1 # simulate failing condition
                        else:
                            print_debug('\tconditional jump detected but no immediate target operand detected :(')

                if curr_instr.mnemonic == 'b' and curr_instr.operands[0].type == ARM_OP_IMM:
                    target = get_branch_dest_idx(instr_list, curr_instr.operands[0].imm)
                    print_debug('\tjump detected, going to index', target)
                    #print('>>>>')
                    #print_insn_detail(curr_instr)
                    #print('<<<<')
                    curr_idx = target
                else:
                    _, regs_write = curr_instr.regs_access()
                    if got_val_reg in regs_write: # overwriting the got_val register means we can stop looking for indirections
                        break
                    elif not cb_flag: # go to next instruction
                        curr_idx += 1

    instr_list = []
    for i in mode.disasm(text_bytes, text_section['sh_addr']):
        #print_insn_detail(i)
        instr_list.append(i)

    for i, _ in enumerate(instr_list):
        r = get_got_load_register(instr_list, i)
        if r:
            print_debug('---------------------------------')
            print_debug('GOT LOAD REG', r)
            print_debug('---------------------------------')

            search_and_destroy_indirections(instr_list, i, r)


# For resolved relocations, change dynamic relocation type from R_ARM_RELATIVE
# to R_ARM_NONE The format of the .rel.dyn section is a simple list of 8-byte
# entries: ([32 bit offset into GOT] [32 bit info field])
def patch_dyn_relocations(e, e_contents, relocations):

    rel_dyn = e.get_section_by_name('.rel.dyn')
    if not rel_dyn:
        return
    rel_dyn_base_offset = rel_dyn['sh_offset']

    for i in range(rel_dyn.num_relocations()):
        relocation_entry = rel_dyn.get_relocation(i)
        for r in relocations:
            if r['got_offset'] == relocation_entry['r_offset']:
                write_to_elf(e_contents, rel_dyn_base_offset + (i * 8) + 4, enums.ENUM_RELOC_TYPE_ARM['R_ARM_NONE'], 4)


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
     
    elf_in_file.seek(0)
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

ENABLE_DEBUG = False

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
    
    if secure_map and extern_map:
        relocate(input_elf, output_elf, extern_map, secure_map, temp_dump, temp_readelf)
    else:
        print("cp " + args.input + " " + args.output)
        status = subprocess.call("cp " + args.input + " " + args.output, shell=True)

if extern_map:
    extern_map.close()

if secure_map:
    secure_map.close()

print_info('relocated PIC app saved as ' + args.output)

