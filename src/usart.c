#include "usart.h"

#include <avr/interrupt.h>
typedef volatile struct CircBuffer {
   volatile char buf[128];
   volatile uint8_t read;
   volatile uint8_t write;
} CircBuffer;

CircBuffer tx = {.read = 0,.write = 0};
CircBuffer rx = {.read = 0, .write = 0};


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

   UCSR0B |= 1 << RXCIE0 ;
   // UCSR0A |= 1 << TXC0;
}



ISR(USART_RX_vect){
   rx.buf[rx.write] = UDR0;
   rx.write = incptr(rx.write);
}

void write_byte(){
   if(tx.read != tx.write){
      UDR0 = tx.buf[tx.read];
      tx.read = incptr(tx.read);
   } else {
      UCSR0B &= (uint8_t) ~(1<< UDRIE0); // Disable iterrupt when not transmitting data
   }
}

uint8_t write( char* data, uint8_t length){
   uint8_t buf = tx.write;
   for(uint8_t i = 0; i < length; i++){
      tx.buf[tx.write] = data[i];
      tx.write = incptr(tx.write);
      if(tx.write == tx.read){
         tx.write = buf;
         return -1;
      }
   };
   // write_byte();
   UCSR0B |= 1 << UDRIE0; // Enable intterupt for transmitting data
   return length;
}

char read(){
   if( rx.write != rx.read){
      char tmp = rx.buf[rx.read];
      rx.read = incptr(rx.read);
      return tmp;
   }
   return -1;
}
ISR(USART_UDRE_vect){
   write_byte();
}