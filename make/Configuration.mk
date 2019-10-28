# Configuration build parameters. Heavily influenced by (read: ripped from)
# github.com/lab11/nrf52x-base.
# 
# Included by AppMakefile.mk

ifndef CONFIGURATION_MAKEFILE
CONFIGURATION_MAKEFILE = 1

# --- Build tools ---

BUILDDIR ?= _build/

# Remove built-in rules and variables (no-op for make version < 4.0)
MAKEFLAGS += -r
MAKEFLAGS += -R

# Toolchain
TOOLCHAIN := arm-none-eabi
AR := $(TOOLCHAIN)-ar
AS := $(TOOLCHAIN)-as
CC := $(TOOLCHAIN)-gcc
CXX := $(TOOLCHAIN)-g++
GDB := $(TOOLCHAIN)-gdb
LD := $(TOOLCHAIN)-gcc
OBJCOPY := $(TOOLCHAIN)-objcopy
OBJDUMP := $(TOOLCHAIN)-objdump
RANLIB := $(TOOLCHAIN)-ranlib
READELF := $(TOOLCHAIN)-readelf
SIZE := $(TOOLCHAIN)-size

# Git version
GIT_VERSION := $(shell git describe --abbrev=4 --always --tags)
BARE_VERSION := $(lastword $(subst v, , $(firstword $(subst -, ,$(GIT_VERSION)))))

# Pretty-printing rules
# If env variable V is non-empty, be verbose
ifneq ($(V),)
Q=
TRACE_AR  =
TRACE_AS  =
TRACE_BIN =
TRACE_CC  =
TRACE_CXX =
TRACE_DEP =
TRACE_DIR =
TRACE_HEX =
TRACE_LD  =
TRACE_LST =
TRACE_SIZ =
else
Q=@
TRACE_AR  = @echo "  AR        " $@
TRACE_AS  = @echo "  AS        " $<
TRACE_BIN = @echo " BIN        " $@
TRACE_CC  = @echo "  CC        " $<
TRACE_CXX = @echo " CXX        " $<
TRACE_DEP = @echo " DEP        " $<
TRACE_DIR = @echo " DIR        " $@
TRACE_HEX = @echo " HEX        " $@
TRACE_LD  = @echo "  LD        " $@
TRACE_LST = @echo " LST        " $<
TRACE_SIZ = @echo " SIZ        " $<
endif

# TODO more configuration vars

# --- Info Dump ---

# Print useful info
space :=
space +=
$(info BUILD OPTIONS:)
$(info $(space) Version    $(GIT_VERSION))
# more info here!
$(info $(space) Board      $(BOARD))
$(info $(space))

# Dump configuration for verbose builds
ifneq ($(V),)
	$(info )
	$(info **************************************************)
	$(info LIVE UPDATE -- VERBOSE BUILD)
	$(info **************************************************)
	$(info Config:)
	$(info TOOLCHAIN=$(TOOLCHAIN))
	$(info CC=$(CC))
	$(info MAKEFLAGS=$(MAKEFLAGS))
	$(info )
	$(info $(CC) --version = $(shell $(CC) --version))
	$(info **************************************************)
	$(info )
endif

# --- Shared compile flags ---
#
# These flags should be propagated to individual board builds (e.g. external
# CMake projects like TF-M)

override CFLAGS += \
	#-Wall \
	# TODO

override OPTIMIZATION_FLAG ?= -Os

override LDFLAGS += \
	# TODO

override LDLIBS += \
	# TODO

override OBJDUMP_FLAGS += \
	--disassemble-all \
	--source \
	--disassembler-options=force-tumb\
	-C\
	--section-headers

# XXX someday pull in nrf52x-base extra CFLAGS

endif
