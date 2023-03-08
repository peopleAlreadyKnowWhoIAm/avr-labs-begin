#include "usart.h"

#include <avr/interrupt.h>

static uint8_t rbuf[128];
static uint8_t rwpointer = 0;
static uint8_t rrpointer =0;

static volatile char* wpointer = 0;
static volatile char* wpointer_end = 0;



inline uint8_t incptr(uint8_t ptr) {
   ptr++;
   if (ptr == 128){
      ptr = 0;
   }
   return ptr;
}

void usart_init() {
   UBRR0H = (uint8_t) UBRR >> 8;
   UBRR0L = (uint8_t) UBRR;

   UCSR0C = 0b11 << UCSZ00;
   UCSR0B = 1 << RXEN0 | 1 << TXEN0;

   UCSR0B |= 1 << RXCIE0 | 1 << UDRE0;
   // UCSR0A |= 1 << TXC0;
}



ISR(USART_RX_vect){
   // rbuf[rwpointer] = UDR0;
   // rwpointer = incptr(rwpointer);
   UDR0 = UDR0;
}

void write_byte(){
   if(wpointer < wpointer_end){
      UDR0 = *wpointer;
      wpointer++;
   }
}

uint8_t wre(char* data, uint8_t length){
   if (!(UCSR0A & (1<<UDRE0))){
      return -1;
   }
   wpointer = data;
   wpointer_end = wpointer+length;
   write_byte();
   return length;
}

char read(){
   if( rrpointer != rwpointer){
      char tmp = rbuf[rrpointer];
      rrpointer = incptr(rrpointer);
      return tmp;
   }
   return -1;
}
ISR(USART_UDRE_vect){
   write_byte();
}