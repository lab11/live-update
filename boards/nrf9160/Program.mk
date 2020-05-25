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
	nrfjprog --program $(MERGED_HEX) -f NRF91 --snr $(shell nrfjprog --ids) --sectorerase
	nrfjprog --reset
	
.PHONY: serial
serial:
	miniterm.py - 115200 --raw

endif

