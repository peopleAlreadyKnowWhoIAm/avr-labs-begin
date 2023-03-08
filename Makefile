DEVICE = atmega328p # From https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html

SOURCES += src/main.c
SOURCES += src/usart.c

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
ASFLAGS += -mmcu=$(DEVICE)
ASFLAGS += -Wall

# Linker flags

# Compiler flags
CFLAGS += -c
CFLAGS += -Os
CFLAGS += -I $(INC)

TARGET = main


OBJS = $(patsubst %.c, %.o, $(SOURCES))

.PHONY: all clean flash

all: $(TARGET).hex

%.o: %.c
	$(CC) $(CFLAGS) $(ASFLAGS) $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $(ASFLAGS) $^ -o $@

%.hex: %.elf
	$(OC) -S -O ihex $< $@
	$(OS) $<

clean:
	rm -f $(OBJS) $(TARGET).hex $(TARGET).elf

flash: $(TARGET).hex
	avrdude -c arduino -P /dev/ttyUSB0 -p $(DEVICE) -U flash:w:$(TARGET).hex