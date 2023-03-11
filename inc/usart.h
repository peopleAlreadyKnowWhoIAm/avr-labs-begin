#ifndef _USART_H_
#define _USART_H_

#include "main.h"
#include "avr/io.h"

#define BAUDRATE 9600

#define UBRR ( FOSC / 16 / BAUDRATE - 1)

#define BUSY_TRANSMITTER (!(UCSR0A & (1<< TXC0)))

void usart_init();

int8_t serial_write_char(const char data);

char serial_read_char();

#endif