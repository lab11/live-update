# Commands and configurations for loading code onto the Musca-A board
# Included by Board.mk

# Ensure that this file is only included once (this file doesn't use JTAG, but
# many do)
ifndef JTAG_MAKEFILE
JTAG_MAKEFILE = 1

# Default port for GDB
GDB_PORT_NUMBER ?= 2331

.PHONY: flash
flash:
	$(Q)cp $(VERSION_FILE) $(FLASHED_VERSION_FILE)
	$(Q)rm -rf $(FLASHED_DIR)
	$(Q)cp -r $(UPDATE_DIR) $(FLASHED_DIR)
	nrfjprog --eraseall -f NRF91
	nrfjprog --program $(MERGED_HEX) -f NRF91
	nrfjprog --reset

.PHONY: erase
erase:
	nrfjprog --eraseall -f NRF91
	
.PHONY: serial
serial:
	miniterm.py - 115200 --raw

endif

