/*
 * user_input.c
 *
 * Create on: 22.11.2014
 * 		Author: popai
 *
 */

#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include<stdlib.h>
#include <string.h>

#include "lib/keypad/keypad.h"
#include "password_manager.h"
#include "user_input.h"
#include "pinDef.h"

#define	DEBUG	1

uint8_t armat = ON;
uint8_t alarm = OFF;


/**
 * @breaf start the alarm
 * and signal a led on bord
 * @param
 */
void ALARMOn()
{
	if (armat && (!alarm))
	{
		ALARM_PORT &= (~(1 << ALARM_PIN));
		alarm = ON;
	}
}

/**
 * @breaf stop the alarm
 *
 * @param none
 */
void ALARMOff()
{
	ALARM_PORT |= (1 << ALARM_PIN);
	alarm = OFF;

}

/**
 * @breaf activate the alarma
 *
 * @param none
 */
void ARMOn()
{
	armat = ON;
	ARMLED_PORT |= (1 << ARMLED_PIN);
	//WriteStateToEEPROM(armat);
	//eeprom_update_byte((uint8_t*) 3, armat);
}

/**
 * @breaf dezactivate the alarma
 *
 * @param none
 */
void ARMOff()
{
	armat = OFF;
	ARMLED_PORT &= (~(1 << ARMLED_PIN));
	//WriteStateToEEPROM(armat);
	//eeprom_update_byte((uint8_t*) 3, armat);
	//SENZOR_PINS &= (~(1 << SENZOR_PIN));
}


uint8_t GetArmat()
{
	return armat;
}

uint8_t GetAlarm()
{
	return alarm;
}

/**
 * @breaf System initialization function
 * 	on this functin init posrts, usart, timer amd...
 *
 * @param no parameter
 */


void Buzer_PassOK()
{
	BUZER_PORT |= (1 << BUZER_PIN);
	_delay_ms(50);
	BUZER_PORT &= (~(1 << BUZER_PIN));
	_delay_ms(50);
	BUZER_PORT |= (1 << BUZER_PIN);
	_delay_ms(50);
	BUZER_PORT &= (~(1 << BUZER_PIN));
}

void Buzer_PassNotOK()
{
	BUZER_PORT |= (1 << BUZER_PIN);
	_delay_ms(50);
	BUZER_PORT &= (~(1 << BUZER_PIN));
	_delay_ms(100);
	BUZER_PORT |= (1 << BUZER_PIN);
	_delay_ms(50);
	BUZER_PORT &= (~(1 << BUZER_PIN));
	_delay_ms(100);
	BUZER_PORT |= (1 << BUZER_PIN);
	_delay_ms(50);
	BUZER_PORT &= (~(1 << BUZER_PIN));
	_delay_ms(100);
	BUZER_PORT |= (1 << BUZER_PIN);
	_delay_ms(50);
	BUZER_PORT &= (~(1 << BUZER_PIN));
	_delay_ms(100);
	BUZER_PORT |= (1 << BUZER_PIN);
	_delay_ms(50);
	BUZER_PORT &= (~(1 << BUZER_PIN));
	_delay_ms(100);
	BUZER_PORT |= (1 << BUZER_PIN);
	_delay_ms(50);
	BUZER_PORT &= (~(1 << BUZER_PIN));

}


