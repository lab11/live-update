# Master application makefile. Heavily influenced by (read: ripped from)
# github.com/lab11/nrf52x-base.

# The first target Make finds is its default. So this line needs to be first to
# specify `all` as our default rule
all:

OUTPUT_NAME ?= $(PROJECT_NAME)
NONSECURE_SUFFIX = _ns
SECURE_SUFFIX = _s

MERGED_HEX = $(BUILDDIR)$(OUTPUT_NAME)_merged.hex

HEX = $(BUILDDIR)$(OUTPUT_NAME)$(NONSECURE_SUFFIX).hex
ELF = $(BUILDDIR)$(OUTPUT_NAME)$(NONSECURE_SUFFIX).elf
BIN = $(BUILDDIR)$(OUTPUT_NAME)$(NONSECURE_SUFFIX).bin
LST = $(BUILDDIR)$(OUTPUT_NAME)$(NONSECURE_SUFFIX).lst
MAP = $(BUILDDIR)$(OUTPUT_NAME)$(NONSECURE_SUFFIX).map

HEX_S = $(BUILDDIR)$(OUTPUT_NAME)$(SECURE_SUFFIX).hex
ELF_S = $(BUILDDIR)$(OUTPUT_NAME)$(SECURE_SUFFIX).elf
BIN_S = $(BUILDDIR)$(OUTPUT_NAME)$(SECURE_SUFFIX).bin
LST_S = $(BUILDDIR)$(OUTPUT_NAME)$(SECURE_SUFFIX).lst
MAP_S = $(BUILDDIR)$(OUTPUT_NAME)$(SECURE_SUFFIX).map

# Include supporting makefiles

# TODO explain contents
include $(BASE_DIR)/make/Configuration.mk

# include $(BASE_DIR)/make/Includes.mk
# include $(BASE_DIR)/make/Program.mk

# Board-specific configuration
# TODO explain contents
include $(BASE_DIR)/boards/$(BOARD)/Board.mk

# --- Rules for building apps ---

.PHONY: all
all: $(HEX_S) $(MERGED_HEX)

$(BUILDDIR):
	$(TRACE_DIR)
	$(Q)mkdir -p $@

$(HEX_S): $(ELF_S) | $(BUILDDIR)
	$(TRACE_HEX)
	$(Q)$(OBJCOPY) -Oihex $< $(HEX_S)
	$(TRACE_BIN)
	$(Q)$(OBJCOPY) -Obinary $< $(BIN_S)
	$(TRACE_SIZ)
	$(Q)$(SIZE) $<

$(HEX): $(ELF) | $(BUILDDIR)
	$(TRACE_HEX)
	$(Q)$(OBJCOPY) -Oihex $< $(HEX)
	$(TRACE_BIN)
	$(Q)$(OBJCOPY) -Obinary $< $(BIN)
	$(TRACE_SIZ)
	$(Q)$(SIZE) $<

$(MERGED_HEX): $(HEX) $(HEX_S)
	# TODO merge rule

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

endif
