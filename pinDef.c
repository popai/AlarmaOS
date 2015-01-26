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
	DDRC  = 0b00001111; // PC0, PC1, PC2 out for keybord; PC3 out buzer keybord; PC4 and PC5 in pins DELL and DEBUG
	PORTC = 0b11110000;

	DDRB  = 0b00110000; // PB0, BP1, PB2, PB3 in for keybord; PB4, PB5 out for LED
	PORTB = 0b11001111;
	//PORTB |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3);

	DDRD  = 0b10000010; // PD2 senzor intarziat, PD4, PD4, PD5 senzori rapizi
	PORTD = 0b01111101;

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

