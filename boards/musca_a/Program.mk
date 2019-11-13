# Commands and configurations for loading code onto the Musca-A board
# Included by Board.mk

# ---- JTAG configurations

# JTAG tools
JLINK = JLinkExe
JLINK_GDBSERVER = JLinkGDBServer
JLINK_RTTCLIENT = JLinkRTTClient

# nrfutil tools
MERGEHEX = mergehex
NRFUTIL = nrfutil

# Default port for GDB
GDB_PORT_NUMBER ?= 2331

.PHONY: flash
flash: all
	cp $(MERGED_HEX) /Volumes/MUSCA/

.PHONY: serial
serial:
	miniterm.py - 115200 --raw

