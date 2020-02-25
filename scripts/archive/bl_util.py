import argparse
import sys

parser = argparse.ArgumentParser(description='Utility for function call translation (ARMv8-M ISA)')
parser.add_argument('-b', '--branch', type=lambda x: int(x,0), help='(hex) branch instruction to modify', required=True)
parser.add_argument('-o', '--old-addr', type=lambda x: int(x,0), help='(hex) old instruction address', required=True)
parser.add_argument('-i', '--new-addr', type=lambda x: int(x,0), help='(hex) new instruction address', required=True)
args = parser.parse_args()
print()

instr = args.branch
if (instr >> 27 == 0b11110) and ((instr & 0x000d000) >> 12 == 0b1101):
    print('Parsing `BL` (Type 1)')
    print()
    print('  ' + format(instr >> 16, 'x') + ' ' + format(instr & 0xffff, 'x'))

    s = (instr >> 26) & 1
    print('  s      = ' + bin(s))
    imm10 = (instr >> 16) & 0x3ff
    print('  imm10  = ' + bin(imm10))
    j1 = (instr >> 13) & 1
    print('  j1     = ' + bin(j1))
    j2 = (instr >> 11) & 1
    print('  j2     = ' + bin(j2))
    imm11 = instr & 0x7ff
    print('  imm11  = ' + bin(imm11))

    i1 = 0 if j1^s else 1
    print('  i1     = ' + bin(i1))
    i2 = 0 if j2^s else 1
    print('  i2     = ' + bin(i1))

    # sign extension by mask
    imm32 = 0xfe000000 if s == 1 else 0x00000000
    imm32 = imm32 | s << 24 | i1 << 23 | i2 << 22 | imm10 << 12 | imm11 << 1
    print('  imm32  = ' + bin(imm32))
    print('  offset = ' + hex(imm32 + 4))

    print()
    destination = args.old_addr + imm32 + 4
    print('  branch destination: ' + hex(destination))
    print()

    # convert to new instruction
    print('Generating same `BL` instruction, located at address ' + hex(args.new_addr)) 
    print()
    new_imm32 = destination - args.new_addr - 4
    print('  new offset     = ' + hex(new_imm32 + 4))
    print('  new_imm32      = ' + bin(new_imm32))
    
    new_imm32 = new_imm32 >> 1
    new_imm11 = new_imm32 & 0x7ff
    print('  new_imm11      = ' + bin(new_imm11))
    new_imm32 = new_imm32 >> 11
    new_imm10 = new_imm32 & 0x3ff
    print('  new_imm10      = ' + bin(new_imm10))
    new_imm32 = new_imm32 >> 10
    new_i2 = new_imm32 & 0x1
    print('  new_i2         = ' + bin(new_i2))
    new_imm32 = new_imm32 >> 1
    new_i1 = new_imm32 & 0x1
    print('  new_i1         = ' + bin(new_i1))
    new_imm32 = new_imm32 >> 1
    new_s = new_imm32 & 0x1
    print('  new_s          = ' + bin(new_s))

    new_j1 = (0 if new_i1 else 1)^new_s
    print('  new_j1         = ' + bin(new_j1))
    new_j2 = (0 if new_i2 else 1)^new_s
    print('  new_j2         = ' + bin(new_j2))

    new_branch = 0xf000d000 # fixed bits (see arch. manual)
    new_branch = new_branch | new_s << 26 | new_imm10 << 16 | new_j1 << 13 | new_j2 << 11 | new_imm11

    print()
    print('  ' + format(new_branch >> 16, 'x') + ' ' + format(new_branch & 0xffff, 'x'))

else:
    print('Not recognized')

print()
