srec_cat install/outputs/MUSCA_A/mcuboot.bin -Binary -offset 0x200000 tfm_sign.bin -Binary -offset 0x220000 -o tfm.hex -Intel && \
cp tfm.hex /Volumes/MBED/ # && \
# python app/scripts/elf2update.py --asfile cmake_build/update.s --updatefile cmake_build/update.bin cmake_build/install/outputs/MUSCA_A/tfm_ns.axf

