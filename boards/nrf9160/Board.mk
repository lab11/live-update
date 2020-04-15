# nRF9160 application makefile. Heavily influenced by (read: ripped from)
# github.com/lab11/nrf52x-base.

# TODO
#  - version number file
#  - tfm veneers if applicable

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

VERSION_FILE = .lastVerNum.txt

# Include supporting makefiles

include $(BASE_DIR)/boards/$(BOARD)/Program.mk

# --- Rules for building apps ---

.PHONY: all
all: init_zephyr_env $(MERGED_HEX)

.PHONY: init_zephyr_env
init_zephyr_env:
	$(shell export ZEPHYR_BASE=$(ZEPHYR_BASE))
	$(shell source $(ZEPHYR_BASE)/zephyr-env.sh)

$(BUILDDIR):
	$(Q)mkdir -p $@

$(ZEPHYR_PRJ_CONFIG):
	$(Q)python3 $(BASE_DIR)/make/gen_prj_conf.py $(BASE_DIR)/apps/$(PROJECT_NAME) > $@

$(ZEPHYR_APP_LINKER_SCRIPT): $(BUILDDIR)
	$(Q)cp $(BASE_DIR)/make/app-sections.ld $@

$(ZEPHYR_CMAKELISTS): Makefile $(ZEPHYR_APP_LINKER_SCRIPT) $(ZEPHYR_PRJ_CONFIG)
	$(Q)python3 $(BASE_DIR)/make/gen_zephyr_cmake.py $(PROJECT_NAME) "$(APP_SOURCES)" --include_dirs "$(APP_HEADER_PATHS)" > $@

#VERSION_NUM = $(shell test -f ${VERSION_FILE} && tail -c 1 ${VERSION_FILE})
#ifeq ($(VERSION_NUM),)
#	VERSION_NUM = 0
#endif
#PARTITION = $(shell echo ${VERSION_NUM}%2 | bc)
#
#ZEPHYR_BOARD = v2m_musca_nonsecure
#ifeq ($(PARTITION),1)
#ZEPHYR_BOARD = v2m_musca_nonsecure_p1
#endif
PARTITION = 0
ZEPHYR_BOARD = nrf9160_pca10090ns

$(MERGED_HEX): $(BUILDDIR) $(ZEPHYR_CMAKELISTS)
	@echo " Building application for PARTITION $(PARTITION)..."
	cd $(ZEPHYR_BASE) && west build -p=auto -b $(ZEPHYR_BOARD) $(APP_DIR)/
	$(Q)cp $(ZEPHYR_BASE)/build/zephyr/merged.hex $@

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

