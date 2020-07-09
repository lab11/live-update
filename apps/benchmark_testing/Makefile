PROJECT_NAME = $(shell basename "$(realpath ./)")

# Configuration
BOARD = nrf9160

# Source and header files
APP_HEADER_PATHS += ./inc
APP_SOURCE_PATHS += ./src
APP_SOURCES = $(wildcard ./src/*.c)

# Include main makefile
BASE_DIR ?= ../..
include $(BASE_DIR)/make/AppMakefile.mk

