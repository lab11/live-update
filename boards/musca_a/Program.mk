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
	$(Q)python3 $(BASE_DIR)/scripts/save_update_info.py $(BASE_DIR)/apps/$(PROJECT_NAME)
	$(Q)cp $(MERGED_HEX) /Volumes/MUSCA/

.PHONY: serial
serial:
	miniterm.py - 115200 --raw

endif

