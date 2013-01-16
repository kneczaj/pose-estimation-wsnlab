# Target platform
PLATFORM = micaZ

# Target file name (without extension).
TARGET = <your c main file>

# Set the Port that you programmer is connected to
USB ?= 0
PROGRAMMING_PORT = /dev/ttyUSB$(USB) # programmer connected to serial device. For Windows use for example COM23

# Set this such that the nano-RK directory is the base path
# In the lab it is $NANORK
ROOT_DIR = $(NANORK)

# Set platform specific defines
# The following will be defined based on the PLATFORM variable:
# PROG_TYPE  (e.g. avrdude, or uisp)
# MCU (e.g. atmega32, atmega128, atmega1281)
# RADIO (e.g. cc2420)

include $(ROOT_DIR)/include/platform.mk

SRC = $(TARGET).c

# Add extra source files.
# For example:
# SRC += $(ROOT_DIR)/src/platform/$(PLATFORM_TYPE)/source/my_src1.c
# SRC += other.c
# SRC += $(ROOT_DIR)/src/platform/$(PLATFORM_TYPE)/source/my_src2.c

# Add extra includes files.
# For example:
# EXTRAINCDIRS += $(ROOT_DIR)/src/platform/include

EXTRAINCDIRS =

#  This is where the final compile and download happens
include $(ROOT_DIR)/include/platform/$(PLATFORM)/common.mk