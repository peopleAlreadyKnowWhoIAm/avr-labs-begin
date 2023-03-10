#ifndef _USART_H_
#define _USART_H_

#include "main.h"
#include "avr/io.h"

#define BAUDRATE 9600

#define UBRR ( FOSC / 16 / BAUDRATE - 1)

#define BUSY_TRANSMITTER (!(UCSR0A & (1<< TXC0)))

void usart_init();

uint8_t write(char* data, uint8_t length);

char read();

#endif