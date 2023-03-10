#include "main.h"
#include "usart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DELAY 600
// Button input pullup
#define BUTTON_Mask (1<<PB2)

const uint8_t sequence[] = {
    1<<PD0,
    1<<PD2,
    1<<PD4,
    1<<PD6,
    1<<PD1,
    1<<PD3,
    1<<PD5,
    1<<PD7,
    0
};

void setup();

int main() {
    setup();
    if(write("alpha", 5) == 5){
        PORTB |= 1<<PB5;
    }
    char buf[10];
    uint8_t len = 0;
    while(1){
        char tmp = read();
        if (tmp != -1){
            buf[len] = tmp;
            len++;
        }

        if( len > 5 && write(buf, len) != -1){
            
            len = 0;
        }
        // if (!(PINB & (1 << PIN2))){
        //     for(uint8_t i = 0; i < 9; i++){
        //         PORTD = sequence[i];
        //         _delay_ms(DELAY);
        //     }
        // }
        
    }

    return 0;
}

void setup(){
    //Button
    // DDRB &= ~(BUTTON_Mask); // Make sure input
    // PORTB |= BUTTON_Mask; // Pull up

    // // leds
    // DDRD = 0xff; //Set all port to output 
    DDRB |= 1 << PB5;

    usart_init();

    sei();

}