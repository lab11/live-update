# Musca-A application makefile. Heavily influenced by (read: ripped from)
# github.com/lab11/nrf52x-base.

# The first target Make finds is its default. So this line needs to be first to
# specify `all` as our default rule
all:

OUTPUT_NAME ?= $(PROJECT_NAME)
NONSECURE_SUFFIX = _ns
SECURE_SUFFIX = _s
DEBUG_SUFFIX = _debug

ARM_TFM_DIR = $(BASE_DIR)/ext/trusted-firmware-m
ZEPHYR_BASE = $(BASE_DIR)/ext/zephyros/zephyr
ZEPHYR_CMAKELISTS = CMakeLists.txt
ZEPHYR_PRJ_CONFIG = prj.conf
ZEPHYR_APP_LINKER_SCRIPT = $(BUILDDIR)app-sections.ld
ZEPHYR_BUILDDIR = $(ZEPHYR_BASE)/build

MERGED_BIN = $(BUILDDIR)$(OUTPUT_NAME)_merged.bin
SIGNED_BIN = $(BUILDDIR)$(OUTPUT_NAME)_merged_signed.bin
MERGED_HEX = $(BUILDDIR)$(OUTPUT_NAME)_merged.hex

HEX = $(BUILDDIR)$(OUTPUT_NAME)$(NONSECURE_SUFFIX).hex
DEBUG_HEX = $(BUILDDIR)$(OUTPUT_NAME)$(NONSECURE_SUFFIX)$(DEBUG_SUFFIX).hex
ELF = $(BUILDDIR)$(OUTPUT_NAME)$(NONSECURE_SUFFIX).elf
DEBUG_ELF = $(BUILDDIR)$(OUTPUT_NAME)$(NONSECURE_SUFFIX)$(DEBUG_SUFFIX).elf
BIN = $(BUILDDIR)$(OUTPUT_NAME)$(NONSECURE_SUFFIX).bin
DEBUG_BIN = $(BUILDDIR)$(OUTPUT_NAME)$(NONSECURE_SUFFIX)$(DEBUG_SUFFIX).bin
LST = $(BUILDDIR)$(OUTPUT_NAME)$(NONSECURE_SUFFIX).lst
MAP = $(BUILDDIR)$(OUTPUT_NAME)$(NONSECURE_SUFFIX).map

HEX_S = $(BUILDDIR)$(OUTPUT_NAME)$(SECURE_SUFFIX).hex
DEBUG_HEX_S = $(BUILDDIR)$(OUTPUT_NAME)$(SECURE_SUFFIX)$(DEBUG_SUFFIX).hex
ELF_S = $(BUILDDIR)$(OUTPUT_NAME)$(SECURE_SUFFIX).elf
DEBUG_ELF_S = $(BUILDDIR)$(OUTPUT_NAME)$(SECURE_SUFFIX)$(DEBUG_SUFFIX).elf
BIN_S = $(BUILDDIR)$(OUTPUT_NAME)$(SECURE_SUFFIX).bin
DEBUG_BIN_S = $(BUILDDIR)$(OUTPUT_NAME)$(SECURE_SUFFIX)$(DEBUG_SUFFIX).bin
LST_S = $(BUILDDIR)$(OUTPUT_NAME)$(SECURE_SUFFIX).lst
MAP_S = $(BUILDDIR)$(OUTPUT_NAME)$(SECURE_SUFFIX).map

VERSION_FILE = .lastVerNum.txt
FLASHED_VERSION_FILE= $(BUILDDIR)lastFlashedNum.txt

UPDATE_DIR = $(BUILDDIR)update
FLASHED_SYMBOLS = $(BUILDDIR)flashed.symbols
AST_DUMP = $(BUILDDIR)ast_dump.txt

# Include supporting makefiles

include $(BASE_DIR)/boards/$(BOARD)/Program.mk

# --- Rules for building apps ---

.PHONY: all
all: init_zephyr_env $(BIN_S) $(BIN) $(MERGED_HEX) $(UPDATE_DIR)

.PHONY: init_zephyr_env
init_zephyr_env:
	$(shell export ZEPHYR_BASE=$(ZEPHYR_BASE))
	$(shell source $(ZEPHYR_BASE)/zephyr-env.sh)

.PHONY: $(UPDATE_DIR)
$(UPDATE_DIR): $(BUILDDIR)
	$(TRACE_DIR)
	$(Q)mkdir -p $@
	$(Q)rm -f $@/*
	$(Q)(test -f $(FLASHED_SYMBOLS) && cp $(FLASHED_SYMBOLS) $@/flashed.symbols) || true
	$(Q)arm-none-eabi-objdump -t $(ELF) > $@/update.symbols
	$(Q)cp $(AST_DUMP) $@/update_ast.txt
	$(Q)cp $(ELF) $@/update_ns.elf
	$(Q)cp $(MERGED_HEX) $@/update.hex
	$(Q)python3 $(BASE_DIR)/make/gen_update_manifest.py $@ $(FLASHED_VERSION_FILE) $(VERSION_FILE) > $@/manifest.json

$(BUILDDIR):
	$(TRACE_DIR)
	$(Q)mkdir -p $@
	$(Q)mkdir -p $@/arm-tfm

$(ZEPHYR_PRJ_CONFIG):
	$(Q)python3 $(BASE_DIR)/make/gen_prj_conf.py $(BASE_DIR)/apps/$(PROJECT_NAME) > $@

$(ZEPHYR_APP_LINKER_SCRIPT): $(BUILDDIR)
	$(Q)cp $(BASE_DIR)/make/app-sections.ld $@

$(ZEPHYR_CMAKELISTS): Makefile $(ZEPHYR_APP_LINKER_SCRIPT) $(ZEPHYR_PRJ_CONFIG)
	$(Q)python3 $(BASE_DIR)/make/gen_zephyr_cmake.py $(PROJECT_NAME) "$(APP_SOURCES) ./_build/arm-tfm/install/export/tfm/veneers/s_veneers.o" --include_dirs "$(APP_HEADER_PATHS) ./_build/arm-tfm/install/export/tfm/inc" > $@
		
$(ELF_S): $(BUILDDIR)
	$(Q)cmake $(ARM_TFM_DIR) -B $(BUILDDIR)arm-tfm -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DTARGET_PLATFORM=MUSCA_A -DCOMPILER=GNUARM	
	$(Q)cmake --build $(BUILDDIR)arm-tfm -- install
	$(Q)cp $(BUILDDIR)arm-tfm/install/outputs/MUSCA_A/tfm_s.axf $@


VERSION_NUM = $(shell test -f ${VERSION_FILE} && tail -c 1 ${VERSION_FILE})
ifeq ($(VERSION_NUM),)
	VERSION_NUM = 1
endif
PARTITION = $(shell echo ${VERSION_NUM}%2 | bc)

ZEPHYR_BOARD = v2m_musca_nonsecure
ifeq ($(PARTITION),1)
ZEPHYR_BOARD = v2m_musca_nonsecure_p1
endif

$(ELF): $(BUILDDIR) $(ZEPHYR_CMAKELISTS)
	@echo " Building application for PARTITION $(PARTITION)..."
	cd $(ZEPHYR_BASE) && west build --pristine -b $(ZEPHYR_BOARD) $(APP_DIR)/
	$(Q)cp $(ZEPHYR_BASE)/build/zephyr/zephyr.elf $@

.PHONY: $(BIN_S)
$(BIN_S): $(ELF_S) | $(BUILDDIR)
	$(TRACE_HEX)
	$(Q)$(OBJCOPY) -Oihex $< $(HEX_S)
	$(TRACE_BIN)
	$(Q)$(OBJCOPY) -Obinary $< $(BIN_S)
	$(TRACE_SIZ)
	$(Q)$(SIZE) $<

.PHONY: $(BIN)
$(BIN): $(ELF) | $(BUILDDIR)
	$(TRACE_HEX)
	$(Q)$(OBJCOPY) -Oihex $< $(HEX)
	$(TRACE_BIN)
	$(Q)$(OBJCOPY) -Obinary $< $(BIN)
	$(TRACE_SIZ)
	$(Q)$(SIZE) $<

.PHONY: $(MERGED_BIN)
$(MERGED_BIN): $(BIN_S) $(BIN) 
	$(Q)python3 $(ARM_TFM_DIR)/bl2/ext/mcuboot/scripts/assemble.py \
		-l $(ARM_TFM_DIR)/platform/ext/target/musca_a/partition/flash_layout.h \
		-s $(BIN_S) \
		-n $(BIN) \
		-o $@

.PHONY: $(SIGNED_BIN)
$(SIGNED_BIN): $(MERGED_BIN)
	$(Q)python3 $(ARM_TFM_DIR)/bl2/ext/mcuboot/scripts/imgtool.py \
		sign --layout $(ARM_TFM_DIR)/platform/ext/target/musca_a/partition/flash_layout.h \
		-k $(ARM_TFM_DIR)/bl2/ext/mcuboot/root-rsa-2048.pem \
		--align 1 -H 0x400 --pad 0x100000 $< $@

.PHONY: $(MERGED_HEX)
$(MERGED_HEX): $(SIGNED_BIN)
	$(Q)srec_cat $(BUILDDIR)arm-tfm/bl2/ext/mcuboot/mcuboot.bin -Binary -offset 0x200000 $< -Binary -offset 0x220000 -o $@ -Intel

.PHONY: lst
lst: $(ELF)
	$(TRACE_LST)
	$(Q)$(OBJDUMP) $(OBJDUMP_FLAGS) $< > $(LST)

.PHONY: lst-secure
lst-secure: $(ELF_S)
	$(TRACE_LST)
	$(Q)$(OBJDUMP) $(OBJDUMP_FLAGS) $< > $(LST_S)

.PHONY: size
size: $(ELF)
	$(TRACE_SIZ)
	$(Q)$(SIZE) $<

.PHONY: size-secure
size-secure: $(ELF_S)
	$(TRACE_SIZ)
	$(Q)$(SIZE) $<

.PHONY: clean
clean::
	@echo " Cleaning..."
	$(Q)rm -rf $(BUILDDIR)
	$(Q)rm -rf $(ZEPHYR_BUILDDIR)
	$(Q)rm -f $(ZEPHYR_CMAKELISTS)
	$(Q)rm -f $(ZEPHYR_PRJ_CONFIG)

