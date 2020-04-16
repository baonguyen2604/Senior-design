# Makefile
#
# targets:
#   all:    compiles the source code
#   test:   tests the isp connection to the mcu
#   flash:  writes compiled hex file to the mcu's flash memory
#   fuse:   writes the fuse bytes to the MCU
#   disasm: disassembles the code for debugging
#   clean:  removes all .hex, .elf, and .o files in the source code and library directories

# parameters (change this stuff accordingly)
# project name
PRJ = microcontroller_a
# avr mcu
MCU = atmega328p
# mcu clock frequency
CLK = 16000000
# avr programmer (and port if necessary)
# e.g. PRG = usbtiny -or- PRG = arduino -P /dev/tty.usbmodem411
PRG = usbtiny
PORT = /dev/ttyUSB0
BAUD = 57600
# fuse values for avr: low, high, and extended
# these values are from an Arduino Uno (ATMega328P)
# see http://www.engbedded.com/fusecalc/ for other MCUs and options
LFU = 0xFF
HFU = 0xDE
EFU = 0x05
# binaries directory
BIN = ./bin/
# program source files (not including external libraries)
SRC_DIR     = ./src
BUILD_DIR   = ./build
BIN_DIR     = ./bin
SRC_LIST = $(wildcard $(SRC_DIR)/*.c)
OBJ_LIST = $(foreach src, $(SRC_LIST), $(BUILD_DIR)/$(notdir $(src:.c=.o)))
# where to look for external libraries (consisting of .c/.cpp files and .h files)
# e.g. EXT = ../../EyeToSee ../../YouSART
EXT =




#################################################################################################
# \/ stuff nobody needs to worry about until such time that worrying about it is appropriate \/ #
#################################################################################################

# include path
INCLUDE := $(foreach dir, $(EXT), -I$(dir))
# c flags
CFLAGS    = -Wall -Os -DF_CPU=$(CLK) -mmcu=$(MCU) $(INCLUDE) \
			-Wl,-u,vfprintf -lprintf_flt -lm

# executables
AVRDUDE = avrdude -c $(PRG) -p $(MCU) -v
AVRDUDE_USBTINY = avrdude -c usbtiny -p $(MCU) -v
AVRDUDE_ARDUINO = avrdude -c arduino -p $(MCU) -v -b $(BAUD) -P $(PORT)
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE    = avr-size
CC      = avr-gcc

# generate list of objects
CFILES    = $(wildcard $(SRC_DIR)/*.c)
EXTC     := $(foreach dir, $(EXT), $(wildcard $(dir)/*.c))
OBJ       = $(CFILES:.c=.o) $(EXTC:.c=.o)

# user targets
# compile all files
all: $(PRJ).hex

# test programmer connectivity
test_arduino:
	sudo $(AVRDUDE_ARDUINO)

test_usbtiny:
	sudo $(AVRDUDE_USBTINY)

# flash program to mcu
flash_arduino: all
	sudo $(AVRDUDE_ARDUINO) -U flash:w:$(BIN_DIR)/$(PRJ).hex:i

# flash program to mcu
flash_usbtiny: all
	sudo $(AVRDUDE_USBTINY) -U flash:w:$(BIN_DIR)/$(PRJ).hex:i

# write fuses to mcu
fuse:
	sudo $(AVRDUDE) -U lfuse:w:$(LFU):m -U hfuse:w:$(HFU):m -U efuse:w:$(EFU):m

# generate disassembly files for debugging
disasm: $(PRJ).elf
	$(OBJDUMP) -d $(BIN_DIR)/$(PRJ).elf

# remove compiled files
clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)
	$(foreach dir, $(EXT), rm -f $(dir)/*.o;)

directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BIN_DIR)

# other targets
# objects from c files
$(OBJ_LIST): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(XCFLAGS) -c $< -o $@

# elf file
$(PRJ).elf: directories $(OBJ_LIST)
	$(CC) $(CFLAGS) $(XCFLAGS) -o $(BIN_DIR)/$(PRJ).elf $(OBJ_LIST)

# hex file
$(PRJ).hex: $(PRJ).elf
	rm -f $(BIN_DIR)/$(PRJ).hex
	$(OBJCOPY) -j .text -j .data -O ihex $(BIN_DIR)/$(PRJ).elf $(BIN_DIR)/$(PRJ).hex
	$(SIZE) $(BIN_DIR)/$(PRJ).elf