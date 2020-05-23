# Commands and configurations for loading code onto the Musca-A board
# Included by Board.mk

# Ensure that this file is only included once (this file doesn't use JTAG, but
# many do)
ifndef JTAG_MAKEFILE
JTAG_MAKEFILE = 1

# Default port for GDB
GDB_PORT_NUMBER ?= 2331

FLASHED_UPDATE_DIR = $(BUILDDIR)flashed

.PHONY: flash
flash:
	$(Q)cp $(VERSION_FILE) $(FLASHED_VERSION_FILE)
	$(Q)arm-none-eabi-objdump -t $(ELF) > $(FLASHED_SYMBOLS)
	$(Q)cp -r $(UPDATE_DIR) $(FLASHED_UPDATE_DIR)
	$(Q)cp $(MERGED_HEX) /Volumes/MUSCA/

.PHONY: serial
serial:
	miniterm.py - 115200 --raw

endif

