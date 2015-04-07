/*
 * pinDef.c
 *
 *  Created on: Apr 14, 2013
 *      Author: popai
 */
#include <avr/io.h>
#include "pinDef.h"


void pinSetUp()
{
	DDRC  = 0b00000111; // PC0, PC1, PC2 OUT for keybord; PC3 IN panick button; PC4 and PC5 IN pins DELL and DEBUG
	PORTC = 0b11111000;

	DDRB  = 0b00110000; // PB0, BP1, PB2, PB3 IN for keybord; PB4 OUT for LED, PB5 fast allarm
	PORTB = 0b11001111;
	//PORTB |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3);

	DDRD  = 0b10000010; // PD2 (INT0) senzor alarma dezactivare cu parola
	PORTD = 0b01111101;
	PORTD &= ~(1 << PD6);
	/*
	//setup External Interrupt INT0
	cli();		// disable global interrupts
	//DDRD |= 1 << PD2; 			// Set PD2 as input (Using for interupt INT0)
	//PORTD |= 1 << PD2;		 	// Enable PD2 pull-up resistor
	EIMSK |= (1 << INT0); 			// Enable INT0 interrupt
	MCUCR |= (1 << ISC00);			// Any logical change on INT0 generates an interrupt request
	sei();		//Enable Global Interrupt
	*/

}

