#include <avr/io.h>

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
    while(1){
        if (!(PINB & (1 << PIN2))){
            for(uint8_t i = 0; i < 9; i++){
                PORTD = sequence[i];
                _delay_ms(DELAY);
            }
        }
        
    }

    return 0;
}

void setup(){
    //Button
    DDRB &= ~(BUTTON_Mask); // Make sure input
    PORTB |= BUTTON_Mask; // Pull up

    // leds
    DDRD = 0xff; //Set all port to output 

}