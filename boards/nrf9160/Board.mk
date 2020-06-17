# nRF9160 application makefile. Heavily influenced by (read: ripped from)
# github.com/lab11/nrf52x-base.

# The first target Make finds is its default. So this line needs to be first to
# specify `all` as our default rule
all:

OUTPUT_NAME ?= $(PROJECT_NAME)

ZEPHYR_BASE = $(BASE_DIR)/ext/ncs/zephyr
ZEPHYR_CMAKELISTS = CMakeLists.txt
ZEPHYR_PRJ_CONFIG = prj.conf
ZEPHYR_APP_LINKER_SCRIPT = $(BUILDDIR)app-sections.ld
ZEPHYR_BUILDDIR = $(ZEPHYR_BASE)/build

MERGED_HEX = $(BUILDDIR)$(OUTPUT_NAME)_merged.hex
ELF = $(BUILDDIR)$(OUTPUT_NAME)_ns.elf

VERSION_FILE = $(BUILDDIR)lastVerNum.txt
FLASHED_VERSION_FILE = $(BUILDDIR)lastFlashedNum.txt

UPDATE_DIR = $(BUILDDIR)update
FLASHED_DIR = $(BUILDDIR)flashed

# Include supporting makefiles

include $(BASE_DIR)/boards/$(BOARD)/Program.mk

# --- Rules for building apps ---

.PHONY: all
all: force_build $(MERGED_HEX) $(UPDATE_DIR)
#all: init_zephyr_env $(MERGED_HEX)

.PHONY: force_build
force_build:
	rm -f $(ELF)
	rm -f $(MERGED_HEX)

#.PHONY: init_zephyr_env
#init_zephyr_env:
#	$(shell export ZEPHYR_BASE=$(ZEPHYR_BASE))
#	$(shell source $(ZEPHYR_BASE)/zephyr-env.sh)

CLANG_ANALYSIS_FLAGS = -Xclang -analyze -Xclang -analyzer-checker=core.LiveUpdate -fsyntax-only
CLANG_COMPILE_FLAGS = -ffreestanding -fno-common -Wall -Wformat -Wformat-security -Wno-format-zero-length -Wno-main -Wno-address-of-packed-member -Wno-pointer-sign -Wpointer-arith -Werror=implicit-int -ffunction-sections -fdata-sections -std=c99
CLANG_INCLUDE_DIRS = -imacros$(ZEPHYR_BASE)/include/toolchain/zephyr_stdint.h \
					 -imacros$(ZEPHYR_BUILDDIR)/zephyr/include/generated/autoconf.h \
					 -I./include \
					 -I$(ZEPHYR_BASE)/include \
					 -I$(ZEPHYR_BUILDDIR)/zephyr/include/generated \
					 -I$(ZEPHYR_BASE)/soc/arm/arm/musca_a \
					 -I$(ZEPHYR_BASE)/ext/hal/cmsis/Core/Include \
					 -isystem $(ZEPHYR_BASE)/lib/libc/minimal/include \
					 -isystem /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/../lib/gcc/arm-none-eabi/9.2.1/include \
					 -isystem /usr/local/gcc-arm-none-eabi-9-2019-q4-major/bin/../lib/gcc/arm-none-eabi/9.2.1/include-fixed

.PHONY: $(UPDATE_DIR)
$(UPDATE_DIR): $(BUILDDIR) $(ELF)
	$(TRACE_DIR)
	$(Q)mkdir -p $@
	$(Q)rm -f $@/*
	$(Q)arm-none-eabi-objdump -t $(ELF) > $@/update.symbols
	$(Q)(arm-none-eabi-objdump -D -s -j.app_data $(ELF) > $@/update.data 2> /dev/null || touch $@/update.data)
	$(Q)cp $(ELF) $@/update_ns.elf
	$(Q)cp $(MERGED_HEX) $@/update.hex
	$(Q)echo "{\"analysis\":[" > $@/analysis.json
	$(Q)clang $(CLANG_ANALYSIS_FLAGS) $(CLANG_COMPILE_FLAGS) $(CLANG_INCLUDE_DIRS) $(APP_SOURCES) 2>> $@/analysis.json
	$(Q)echo "]}" >> $@/analysis.json
	$(Q)python3 $(BASE_DIR)/scripts/gen_app_graph.py $@/update.symbols $(ELF) $@/analysis.json $@/update.graph --dump_dot $@/update.graph.dot
	$(Q)python3 $(BASE_DIR)/make/gen_update_manifest.py $@ $(VERSION_FILE) > $@/manifest.json

$(BUILDDIR):
	$(TRACE_DIR)
	$(Q)mkdir -p $@

.PHONY: $(ZEPHYR_PRJ_CONFIG)
$(ZEPHYR_PRJ_CONFIG):
	$(Q)python3 $(BASE_DIR)/make/gen_prj_conf.py $(BASE_DIR)/apps/$(PROJECT_NAME) $(PARTITION) > $@

$(ZEPHYR_APP_LINKER_SCRIPT): $(BUILDDIR)
	$(Q)cp $(BASE_DIR)/make/app-sections.ld $@

$(ZEPHYR_CMAKELISTS): Makefile $(ZEPHYR_APP_LINKER_SCRIPT) $(ZEPHYR_PRJ_CONFIG)
	$(Q)python3 $(BASE_DIR)/make/gen_zephyr_cmake.py $(PROJECT_NAME) "$(APP_SOURCES)" --include_dirs "$(APP_HEADER_PATHS)" > $@


VERSION_NUM = $(shell test -f ${VERSION_FILE} && tail -c 1 ${VERSION_FILE})
ifeq ($(VERSION_NUM),)
	VERSION_NUM = 0
endif
PARTITION = $(shell echo ${VERSION_NUM}%2 | bc)

ZEPHYR_BOARD = nrf9160dk_nrf9160ns
ifeq ($(PARTITION),1)
ZEPHYR_BOARD = nrf9160dk_nrf9160ns
endif

$(MERGED_HEX): $(BUILDDIR) $(ZEPHYR_CMAKELISTS) $(ELF)
	$(Q)cp $(ZEPHYR_BASE)/build/zephyr/merged.hex $@

$(ELF): 
	@echo " Building application for PARTITION $(PARTITION)..."
	cd $(ZEPHYR_BASE) && west build -p=auto -b $(ZEPHYR_BOARD) $(APP_DIR)/
	$(Q)python3 $(BASE_DIR)/make/bump_version.py $(VERSION_FILE)
	$(Q)cp $(ZEPHYR_BASE)/build/zephyr/zephyr.elf $@

.PHONY: clean
clean::
	@echo " Cleaning..."
	$(Q)rm -rf $(BUILDDIR)
	$(Q)rm -rf $(ZEPHYR_BUILDDIR)
	$(Q)rm -f $(ZEPHYR_CMAKELISTS)
	$(Q)rm -f $(ZEPHYR_PRJ_CONFIG)

