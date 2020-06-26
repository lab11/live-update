# nRF application makefile
PROJECT_NAME = $(shell basename "$(realpath ./)")

# Configurations
NRF_IC = nrf52832
SDK_VERSION = 15
SOFTDEVICE_MODEL = s132

# Source and header files
APP_HEADER_PATHS += ./inc
APP_SOURCE_PATHS += ./src
APP_SOURCES = $(notdir $(wildcard ./src/*.c))

# Path to base of nRF52-base repo
NRF_BASE_DIR = ../../ext/nrf52x-base/

# Include board Makefile (if any)
include $(NRF_BASE_DIR)boards/nrf52dk/Board.mk

# Include main Makefile
include $(NRF_BASE_DIR)make/AppMakefile.mk
