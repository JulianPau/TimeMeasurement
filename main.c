#include <avr/io.h>
#include <avr/interrupt.h>

unsigned long aktualTime = 1;
unsigned long overflowCount = 1;
unsigned long thisOverflowCount = 3;

unsigned int measureModus = 0;
unsigned int firsttime = 1;


int main(void){

    char toggle = 0;        // toggle LED flag
    
  // init pins**********************************************************
       // output pin
    DDRD |= (1 << PD6);     // LED on pin 11 (PD6)
    // input pin
    DDRD &= ~(1 << PD5);    // switch on pin 9 (PD5)
    PORTD |= (1 << PD5);    // enable pull-up resistor

    // TIMER for timemeasurement******************************************
    OCR1A = 0x0100;         // number to count to
    TCCR1A = 0;             // CTC mode
    TIMSK |= (0 << OCIE1A); // Interrupt disable
    TIFR |= (1 << OCF1A);
    //  CTC mode, clk src = clk/8, start timer
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    int test = 0;
    // enable interrupts
    sei();

    while(1){
        if ((PIND & (1 << PD5)) && measureModus == 0) {    // is switch open?
            
            int cnt = 0;

            for(cnt = 0; cnt < thisOverflowCount; cnt++) { 
                // count to 16384
                TCNT1 = 0;  
                
                if (toggle) {           // toggle LED
                    PORTD &= ~(1 << PD6);
                }
                else {
                    PORTD |=  (1 << PD6);
                }
                while (!(TIFR & (1 << OCF1A))) {  // count reached?
                //nthing
                    
                }
                overflowCount = 0;
                // clear flag
                TIFR |= (1 << OCF1A);
                PORTD &= ~(1 << PD6);       
            }   
            toggle = !toggle;

        }else { // switch closed
        //timemeasurement
            PORTD &= ~(1 << PD6); //switch LED off
            
            test = PIND & (1 << PD5);
            measureModus = 1;             
            
            if(firsttime == 1){              
                OCR1A = 0x0100;                
                TCNT1 = 0;
                TIMSK |= (1 << OCIE1A); // Interrupt enable
                firsttime = 0;  
            }
            // button losgelassen timer auslesen
            if (PIND & (1 << PD5)) {
                measureModus = 0;
                firsttime = 1;
                
                aktualTime = TCNT1;
                
                thisOverflowCount = overflowCount;
                overflowCount = 0;
                TIMSK |= (0 << OCIE1A); // Interrupt disable
            }                
        }           
    }
}



// interrupt service routine (ISR) for timer 1 A compare match
ISR (TIMER1_COMPA_vect)
{
/*  static char tog = 0;
    
    // toggle the LED on each interrupt    
    if (tog) {
        tog = 0;
        PORTD &= ~(1 << PD6);
    }
    else {
        tog = 1;
        PORTD |=  (1 << PD6);
    }*/
    ++overflowCount;
}








