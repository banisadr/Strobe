/*
 * L3a1.c
 *
 * Created: 9/28/2015 11:18:56 AM
 * Author : bahra_000
 */ 

#include <avr/io.h>
#include "m_general.h"

void init(void); // Prototype functions
void adc_start(void);

int main(void)
{
	init();
	adc_start();
	m_red(ON);
	float cycle;
    while (1) 
    {
		cycle = 1;
		if(check(PIND,2)){
			cycle += 1;
		}
		if(check(PIND,3)){
			cycle += 2;
		}
		if(check(PIND,4)){
			cycle += 4;
		}
		if(check(PIND,5)){
			cycle += 8;
		}
	OCR1B = OCR1A*(cycle/16);
    }
}

void init(void){ // used to set pins to proper state
	
	set(DDRB,6); // Set B6 to output
	
	clear(DDRD,2); // Set D2-5 to input
	clear(DDRD,3);
	clear(DDRD,4);
	clear(DDRD,5);
	
	set(PORTD,2); // Enable internal pullup resistors
	set(PORTD,3);
	set(PORTD,4);
	set(PORTD,5);
	
	m_clockdivide(4); // Set to 1 MHz

	OCR1A = 0x7A76; // Set OCR1A to 16
	OCR1B = OCR1A/2; // Set OCR1B to half OCR1B
	
	set(TCCR1B,WGM13); // Set timer mode to mode 15: up to OCR1A, PWM mode
	set(TCCR1B,WGM12);
	set(TCCR1A,WGM11);
	set(TCCR1A,WGM10);
	
	set(TCCR1A,COM1B1); // clear B6 at TCNT1 = OCR1B, set B6 at rollover
	clear(TCCR1A,COM1B0);
	
	clear(TCCR1B,CS12); // Set prescaler to /64
	set(TCCR1B,CS11);
	set(TCCR1B,CS10);
	
}

void adc_start(void){
	
	clear(ADMUX,REFS1); // Set reference voltage to Vcc
	set(ADMUX,REFS0);
	
	clear(ADCSRA,ADPS2); // Set prescaler to /8
	set(ADCSRA,ADPS1);
	set(ADCSRA,ADPS0);
	
	set(DIDR0,ADC0D); // Disable Digital input to ADC0
	
	sei(); // Enable global interrupts
	
	set(ADCSRA,ADIE); // Enable interrupt for when conversion is finished
	
	clear(ADCSRA,ADATE); // Turn off 'free-running' mode
	
	clear(ADCSRB,MUX5); // Select ADC0 at pin F0
	clear(ADMUX,MUX2);
	clear(ADMUX,MUX1);
	clear(ADMUX,MUX0);
	
	set(ADCSRA,ADEN); // Enable ADC subsystem
	
	set(ADCSRA,ADSC); // Begin first conversion
}

ISR(ADC_vect){
	OCR1A = 0x7A76/1023.0*ADC;
	set(ADCSRA,ADSC); // Begin conversion
}