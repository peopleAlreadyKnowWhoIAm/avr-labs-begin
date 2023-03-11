#include "usart.h"

#include <avr/interrupt.h>

typedef volatile struct CircBuffer {
  volatile char buf[128];
  uint8_t read;
  uint8_t write;
} CircBuffer;

CircBuffer tx = {.read = 0, .write = 0};
CircBuffer rx = {.read = 0, .write = 0};

inline uint8_t incptr(uint8_t ptr) {
  ptr++;
  if (ptr == 128) {
    ptr = 0;
  }
  return ptr;
}

void usart_init() {
  // Set baud rate
  UBRR0H = (uint8_t)UBRR >> 8;
  UBRR0L = (uint8_t)UBRR;

  // Set data length to 8 bit, no parity, 1 stop bit, asynchronous
  UCSR0C = 0b11 << UCSZ00;

  // Enable rx and tx
  UCSR0B = 1 << RXEN0 | 1 << TXEN0;

  // Enable intterupt on receive complete
  UCSR0B |= 1 << RXCIE0;
}

ISR(USART_RX_vect) {
  rx.buf[rx.write] = UDR0;
  rx.write = incptr(rx.write);
}

void write_byte() {
  if (tx.read != tx.write) {
    UDR0 = tx.buf[tx.read];
    tx.read = incptr(tx.read);
  } else {
    UCSR0B &= (uint8_t) ~(
        1 << UDRIE0);  // Disable iterrupt when not transmitting data
  }
}

/**
 * @brief write char to buffer to transfer
 *
 *
 * @param data char
 * @return 0 - when written, -1 - when fail
 */
inline int8_t serial_write_char(char data) {
  uint8_t buf = tx.write;
  tx.buf[tx.write] = data;

  tx.write = incptr(tx.write);
  if (tx.write == tx.read) {
    tx.write = buf;
    return -1;
  }

  // write_byte();
  UCSR0B |= 1 << UDRIE0;  // Enable intterupt for transmitting data
  return 0;
}

/**
 * @brief read char from serial buffer
 * 
 *
 * @return next char from buffer. When empty - -1.
 */
inline char serial_read_char() {
  if (rx.write != rx.read) {
    char tmp = rx.buf[rx.read];
    rx.read = incptr(rx.read);
    return tmp;
  }
  return -1;
}


ISR(USART_UDRE_vect) { write_byte(); }