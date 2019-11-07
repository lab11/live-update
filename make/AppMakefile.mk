# Master application makefile. Main job is to vector to build-system specific
# makefiles, depending on the board.

BASE_DIR = $(realpath ../../)
APP_DIR = $(realpath ./)

include $(BASE_DIR)/make/Configuration.mk

# Include board-specific makefile
include $(BASE_DIR)/boards/$(BOARD)/Board.mk

