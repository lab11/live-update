# Commands and configurations for loading code onto the Musca-A board
# Included by Board.mk

VERSION_FILENAME = lastVerNum.txt
#LAST_FLASHED_VERISON_FILENAME = $(BUILDDIR)lastFlashedNum.txt

# Ensure that this file is only included once (this file doesn't use JTAG, but
# many do)
ifndef JTAG_MAKEFILE
JTAG_MAKEFILE = 1

# Default port for GDB
GDB_PORT_NUMBER ?= 2331

.PHONY: flash
flash:
	cp $(MERGED_HEX) /Volumes/MUSCA/
	
.PHONY: serial
serial:
	miniterm.py - 115200 --raw

endif

