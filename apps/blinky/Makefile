PROJECT_NAME = $(shell basename "$(realpath ./)")

# Configuration
BOARD = musca_a

# Source and header files
APP_HEADER_PATHS += .
APP_SOURCE_PATHS += .
APP_SOURCES = $(notdir $(wildcard ./*.c))

BASE_DIR ?= ../..
include $(BASE_DIR)/make/AppMakefile.mk

