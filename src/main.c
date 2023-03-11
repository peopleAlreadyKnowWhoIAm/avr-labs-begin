#include "main.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "usart.h"

#define BUTTON_1 PB2
#define BUTTON_2 PB3

#define DELAY 600

static const char sequnce_start_char = 'a';
static const uint8_t ascii_0 = 48;

static const uint8_t sequences[2][9] = {{
                                            1 << 0,
                                            1 << 2,
                                            1 << 4,
                                            1 << 6,
                                            1 << 1,
                                            1 << 3,
                                            1 << 5,
                                            1 << 7,
                                            0,
                                        },
                                        {
                                            1 << 0,
                                            1 << 7,
                                            1 << 1,
                                            1 << 6,
                                            1 << 2,
                                            1 << 5,
                                            1 << 3,
                                            1 << 4,
                                            0,
                                        }};

void setup();
void show_sequence(uint8_t sequence);
void write_leds(uint8_t positions);

int main() {
  setup();

  // flag when was got sequence character
  uint8_t line_last = 0;
  char line[32];
  while (1) {
    // if variable not -1 will start sequence which number is
    int8_t need_start_sequence = -1;

    char tmp = serial_read_char();
    if (tmp != -1) {
      line[line_last] = tmp;
      if (tmp == '\r'){
        if (line[0] == sequnce_start_char) {
          int8_t buf = line[1] - ascii_0;
          if (buf >= 0 && buf <= 1) {
            need_start_sequence = buf;
          }
        }

        line_last = 0;
      } else {
        line_last++;
        if (line_last == 32) {
          line_last = 0;
        }
      }
      
    }

    if (need_start_sequence == -1) {
      if (!(PINB & (1 << BUTTON_1))) {
        need_start_sequence = 0;
      } else if (!(PINB & (1 << BUTTON_2))) {
        need_start_sequence = 1;
      }
    }

    if (need_start_sequence == 0 || need_start_sequence == 1) {
      show_sequence(need_start_sequence);
    }
  }

  return 0;
}

void setup() {
  // Button
  //  DDRB &= ~(BUTTON_Mask); // Make sure input
  //  PORTB |= BUTTON_Mask; // Pull up

  // leds
  // Using 2-7 pins on port D and 0-1 on port B
  DDRD = 0b11111100;  // Set 6 last to output
  DDRB = 0b11;        // Set 2 first to output - else input

  // buttons
  // Using 2-3 pins on portb with pullup
  // Direction made upper>
  PORTB |= 0b11 << PB2;  // Set pullup for 2-3 pins

  usart_init();

  sei();
}

void show_sequence(uint8_t sequence) {
  for (uint8_t i = 0; i < 9; i++) {
    // Clear LEDs
    PORTD &= ~(0b11111100);
    PORTB &= ~(0b11);
    uint8_t leds = sequences[sequence][i];
    PORTD |= leds << 2;  // 6 first for 6 on port d
    PORTB |= leds >> 6;  // 2 last for 2 on port b
    write_leds(leds);
    _delay_ms(DELAY);
  }
}

void write_leds(uint8_t leds) {
  for (uint8_t i = 0; i < 8; i++) {
    serial_write_char(((leds & (1 << i)) >> i) +
                      ascii_0  // convert bitmap to 0 and 1 char sequence
    );
  }
    serial_write_char('\n');
    serial_write_char('\r');
}