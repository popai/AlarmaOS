/*
 * keypad.c
 *
 * Create on: 22.11.2014
 * 		Author: popai
 *
 */

#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>

#include "FreeRTOS.h"
#include "task.h"

#include "keypad.h"
//#include "../timer/timer.h"

/************************************************
 KEYPAD CONNECTIONS
 	 	PC0 PC1 PC2
 PB0 ----1---2---3---
 PB1 ----4---5---6---
 PB2 ----7---8---9---
 PB3 ----*---0---#---
 PB AR INPUT AND PC AR OUTPUT
 *************************************************/

/**
 * @breaf 4x3 keybord function
 * on this aplication foR 4 lines it is use port B first 4 pins as input pins
 * and for 3 colons it is use port C first 3 pins as out pins.
 *
 * @param no parameter
 */
uint8_t GetKeyPressed()
{
	//uint8_t getkb = 255;	//Indicate No key pressed
	TickType_t xLastWakeTime;
	/* The xLastWakeTime variable needs to be initialised with the current tick
	 count.  Note that this is the only time we access this variable.  From this
	 point on xLastWakeTime is managed automatically by the vTaskDelayUntil()
	 API function. */
	xLastWakeTime = xTaskGetTickCount();

	PORTC |= (1 << PC1) | (1 << PC2);
	PORTC &= ~(1 << PC0);
	_delay_ms(10);
	if (!(PINB & (1 << PB3))) //(PINB & (1 << PB3)) == 0
	{
		PORTC |= (1 << PC3); //buzer on
		_delay_ms(50);
		PORTC &= ~(1 << PC3); //buzer off
		_delay_ms(50);
		while(!(PINB & (1 << PB3)))
			vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));
		return 10;
	}

	if (!(PINB & (1 << PB2)))
	{
		PORTC |= (1 << PC3); //buzer on
		_delay_ms(50);
		PORTC &= ~(1 << PC3); //buzer off
		_delay_ms(50);
		while(!(PINB & (1 << PB2)))
			vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));

		return 7;
	}

	if (!(PINB & (1 << PB1)))
	{
		PORTC |= (1 << PC3); //buzer on
		_delay_ms(50);
		PORTC &= ~(1 << PC3); //buzer off
		_delay_ms(50);
		while(!(PINB & (1 << PB1)))
			vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));

		return 4;
	}

	if (!(PINB & (1 << PB0)))
	{
		PORTC |= (1 << PC3); //buzer on
		_delay_ms(50);
		PORTC &= ~(1 << PC3); //buzer off
		_delay_ms(50);
		while(!(PINB & (1 << PB0)))
			vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));

		return 1;
	}

	PORTC |= (1 << PC0) | (1 << PC2);
	PORTC &= ~(1 << PC1);
	_delay_ms(10);
	if (!(PINB & (1 << PB3)))
	{
		PORTC |= (1 << PC3); //buzer on
		_delay_ms(50);
		PORTC &= ~(1 << PC3); //buzer off
		_delay_ms(50);
		while(!(PINB & (1 << PB3)))
			vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));

		return 0;
	}

	if (!(PINB & (1 << PB2)))
	{
		PORTC |= (1 << PC3); //buzer on
		_delay_ms(50);
		PORTC &= ~(1 << PC3); //buzer off
		_delay_ms(50);
		while(!(PINB & (1 << PB2)))
			vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));

		return 8;
	}

	if (!(PINB & (1 << PB1)))
	{
		PORTC |= (1 << PC3); //buzer on
		_delay_ms(50);
		PORTC &= ~(1 << PC3); //buzer off
		_delay_ms(50);
		while(!(PINB & (1 << PB1)))
			vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));

		return 5;
	}
	if (!(PINB & (1 << PB0)))
	{
		PORTC |= (1 << PC3); //buzer on
		_delay_ms(50);
		PORTC &= ~(1 << PC3); //buzer off
		_delay_ms(50);
		while(!(PINB & (1 << PB0)))
			vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));

		return 2;
	}

	PORTC |= (1 << PC0) | (1 << PC1);
	PORTC &= ~(1 << PC2);
	_delay_ms(10);
	if (!(PINB & (1 << PB3)))
	{
		PORTC |= (1 << PC3); //buzer on
		_delay_ms(50);
		PORTC &= ~(1 << PC3); //buzer off
		_delay_ms(50);
		while(!(PINB & (1 << PB3)))
			vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));

		return 11; //getkb = 11;
	}
	if (!(PINB & (1 << PB2)))
	{
		PORTC |= (1 << PC3); //buzer on
		_delay_ms(50);
		PORTC &= ~(1 << PC3); //buzer off
		_delay_ms(50);
		while(!(PINB & (1 << PB2)))
			vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));

		return 9;
	}
	if (!(PINB & (1 << PB1)))
	{
		PORTC |= (1 << PC3); //buzer on
		_delay_ms(50);
		PORTC &= ~(1 << PC3); //buzer off
		_delay_ms(50);
		while(!(PINB & (1 << PB1)))
			vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));

		return 6;
	}
	if (!(PINB & (1 << PB0)))
	{
		PORTC |= (1 << PC3); //buzer on
		_delay_ms(50);
		PORTC &= ~(1 << PC3); //buzer off
		_delay_ms(50);
		while(!(PINB & (1 << PB0)))
			vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));

		return 3;
	}

	return 255; //Indicate No key pressed
}

