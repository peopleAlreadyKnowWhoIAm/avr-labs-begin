DEVICE = atmega328p # From https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html

SOURCES += src/main.c

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
CFLAGS += -D F_CPU=$(FREQUENCY)

TARGET = main


OBJS = $(patsubst %.c, %.o, $(SOURCES))

.PHONY: all clean

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
