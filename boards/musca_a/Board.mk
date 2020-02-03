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
ZEPHYR_BASE = $(BASE_DIR)/zephyros/zephyr
ZEPHYR_CMAKELISTS = CMakeLists.txt

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

# Include supporting makefiles

include $(BASE_DIR)/boards/$(BOARD)/Program.mk

# --- Rules for building apps ---

.PHONY: all
all: $(BIN_S) $(BIN) $(MERGED_HEX)

$(BUILDDIR):
	$(TRACE_DIR)
	$(Q)mkdir -p $@
	$(Q)mkdir -p $@/arm-tfm

$(ZEPHYR_CMAKELISTS): Makefile
	$(Q)python3 $(BASE_DIR)/make/zephyr_cmake_gen.py $(PROJECT_NAME) "$(APP_SOURCES) ./_build/arm-tfm/install/export/tfm/veneers/s_veneers.o" --include_dirs "$(APP_HEADER_PATHS) ./_build/arm-tfm/install/export/tfm/inc" > $@
		
$(ELF_S): $(BUILDDIR)
	$(Q)cmake $(ARM_TFM_DIR) -B $(BUILDDIR)arm-tfm -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DTARGET_PLATFORM=MUSCA_A -DCOMPILER=GNUARM	
	$(Q)cmake --build $(BUILDDIR)arm-tfm -- install
	$(Q)cp $(BUILDDIR)arm-tfm/install/outputs/MUSCA_A/tfm_s.axf $@

$(ELF): $(BUILDDIR) $(ZEPHYR_CMAKELISTS)
	cd $(ZEPHYR_BASE) && west build --pristine -b v2m_musca_nonsecure $(APP_DIR)/
	$(Q)cp $(ZEPHYR_BASE)/build/zephyr/zephyr.elf $@

$(BIN_S): $(ELF_S) | $(BUILDDIR)
	$(TRACE_HEX)
	$(Q)$(OBJCOPY) -Oihex $< $(HEX_S)
	$(TRACE_BIN)
	$(Q)$(OBJCOPY) -Obinary $< $(BIN_S)
	$(TRACE_SIZ)
	$(Q)$(SIZE) $<

$(BIN): $(ELF) | $(BUILDDIR)
	$(TRACE_HEX)
	$(Q)$(OBJCOPY) -Oihex $< $(HEX)
	$(TRACE_BIN)
	$(Q)$(OBJCOPY) -Obinary $< $(BIN)
	$(TRACE_SIZ)
	$(Q)$(SIZE) $<

$(MERGED_BIN): $(BIN_S) $(BIN) 
	python3 $(ARM_TFM_DIR)/bl2/ext/mcuboot/scripts/assemble.py \
		-l $(ARM_TFM_DIR)/platform/ext/target/musca_a/partition/flash_layout.h \
		-s $(BIN_S) \
		-n $(BIN) \
		-o $@

$(SIGNED_BIN): $(MERGED_BIN)
	$(Q)python3 $(ARM_TFM_DIR)/bl2/ext/mcuboot/scripts/imgtool.py \
		sign --layout $(ARM_TFM_DIR)/platform/ext/target/musca_a/partition/flash_layout.h \
		-k $(ARM_TFM_DIR)/bl2/ext/mcuboot/root-rsa-2048.pem \
		--align 1 -H 0x400 --pad 0x100000 $< $@

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
