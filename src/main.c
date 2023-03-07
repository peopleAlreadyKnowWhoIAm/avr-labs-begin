#include <avr/io.h>

#include <util/delay.h>

#define LED 5 // PORTB

void setup();

int main() {
    setup();
    while(1){
        PORTB = 1<<PB5;
        _delay_ms(1000);
        PORTB = 0b0;
        _delay_ms(1000);
    }

    return 0;
}

void setup(){
    DDRB |= 1<<PB5; // LED config

}