# Makefile made by me to compile targets for arduino based on atmega328p chip

DEVICE = atmega328p # From https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html

#SOURCES += src/main.c
SOURCES += $(wildcard src/*.c)

INC += inc

FREQUENCY = 16000000

#Toolchain

TOOLCHAIN = /usr/bin

CC = $(TOOLCHAIN)/avr-gcc
AS = $(TOOLCHAIN)/as
LD = $(TOOLCHAIN)/ld
OC = $(TOOLCHAIN)/avr-objcopy
OD = $(TOOLCHAIN)/objdump
OS = $(TOOLCHAIN)/avr-size

# Assembler flags
ASFLAGS += -Wall
ASFLAGS += -ggdb
# Linker flags
LFLAGS += -ggdb
# Compiler flags
CFLAGS += -c
CFLAGS += -Os
CFLAGS += -I $(INC)
CFLAGS += --param=min-pagesize=0

#Device dependent
DFLAGS = -mmcu=$(DEVICE)

TARGET = main


OBJS = $(patsubst %.c, %.o, $(SOURCES))

.PHONY: all clean flash

all: $(TARGET).hex

%.o: %.c
	$(CC) $(CFLAGS) $(ASFLAGS) $(DFLAGS) $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $(ASFLAGS) $(LFLAGS) $(DFLAGS) $^ -o $@

%.hex: %.elf
	$(OC) -S -O ihex $< $@
	$(OS) $<

clean:
	rm -f $(OBJS) $(TARGET).hex $(TARGET).elf

flash: $(TARGET).hex
	avrdude -c arduino -P /dev/ttyUSB0 -p $(DEVICE) -U flash:w:$(TARGET).hex