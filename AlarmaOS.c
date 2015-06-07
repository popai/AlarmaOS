/*
 * AlarmaOS.c
 *
 * Create on: 28.01.2015
 * 		Author: popai
 *
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* serial interface include file. */
#include "serial.h"

/* project interface include file. */
#include "pinDef.h"
#include "user_input.h"
#include "password_manager.h"

#define	DEBUG	1

#include "lib/keypad/keypad.h"
#include "lib/eeprom/eeprom.h"
#include "lib/timer/timer.h"
#include "lib/sound/sound.h"

#ifdef portHD44780_LCD
/* LCD (Freetronics 16x2) interface include file. */
#include "hd44780.h"
#endif

/*-----------------------------------------------------------*/
/* Create a handle for the serial port. */
extern xComPortHandle xSerialPort;

// Declare Buzezer variables...
volatile uint8_t buzzerFinished; // flag: 0 whilst playing
const int8_t *buzzerSequence;
//uint8_t buzzerInitialized;


/*Alarm variables*/
static uint16_t pass_save = 255;
static uint16_t password = 255;
static uint8_t martor = 0;
extern uint8_t armat;
extern uint8_t alarm;


static void TasKeypad(void *pvParameters); // keibord imput
static void TaskSenzorL(void *pvParameters); // senzor intarziat
static void TaskSenzorR(void *pvParameters); // senzor instant
static void TaskAlarma(void *pvParameters); // actiouni alarma
static void TaskSemnale(void *pvParameters); // actiouni alarma

static void SystemInit();

static TaskHandle_t xTasKeypad = NULL, xTaskAlarma = NULL;

/*-----------------------------------------------------------*/

/* Main program loop */
int main(void) __attribute__((OS_main));

int main(void)
{

	SystemInit();

	xTaskCreate( TasKeypad, (const portCHAR *)"Keypad" // citire tastatura
			, 120// Tested 9 free @ 208
			, NULL, 3, &xTasKeypad);
	// */

	xTaskCreate( TaskAlarma, (const portCHAR *)"Alarma"// setare stare alarma
			, 120// Tested 9 free @ 208
			, NULL, 3, &xTaskAlarma);
	// */
	xTaskCreate( TaskSenzorR, (const portCHAR *)"SenzorR"// senzor rapid
			, 120// Tested 9 free @ 208
			, NULL, 3, NULL);
	// */
	xTaskCreate( TaskSenzorL, (const portCHAR *)"SenzorL"// senzor lent
			, 120// Tested 9 free @ 208
			, NULL, 3, NULL);
	// */
	xTaskCreate( TaskSemnale, (const portCHAR *)"Semnale"// semnale
			, 120// Tested 9 free @ 208
			, NULL, 3, NULL);
	// */

	avrSerialPrintf_P(PSTR("\r\n\nFree Heap Size: %u\r\n"),
			xPortGetFreeHeapSize()); // needs heap_1 or heap_2 for this function to succeed.

	vTaskStartScheduler();

	avrSerialPrint_P(PSTR("\r\n\n\nGoodbye... no space for idle task!\r\n")); // Doh, so we're dead...

#ifdef portHD44780_LCD
	lcd_Print_P(PSTR("DEAD BEEF!"));
#endif
}

/*-----------------------------------------------------------*/

static void TasKeypad(void *pvParameters)
{
	(void) pvParameters;
	;
	TickType_t xLastWakeTime;
	/* The xLastWakeTime variable needs to be initialised with the current tick
	 count.  Note that this is the only time we access this variable.  From this
	 point on xLastWakeTime is managed automatically by the vTaskDelayUntil()
	 API function. */
	xLastWakeTime = xTaskGetTickCount();

	//DDRD |= _BV(DDD3);
	uint8_t keyCode = 255;
	uint8_t i = 0;
	uint8_t key[] = "####";

	while (1)
	{

		vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));

		keyCode = GetKeyPressed(); //Get the keycode of pressed key
		if (keyCode == 255)
			continue;

		if (keyCode == 11)
		{
			i = 0;
			xSerialPrint_P(PSTR("Anulat!\r\n"));
		}
		else if (i < 4 && (keyCode != 10))
		{
			key[i] = keyCode;
			xSerialPrintf_P(PSTR("key[%i] = %i \r\n"), i, keyCode);
			++i;
		}

		if (i == 4 && keyCode == 10)
		{

			i = 0;
			password = (((1000 * key[0]) + (100 * key[1]) + (10 * key[2])
					+ key[3])); //save the password in 16 bit integer
#ifdef DEBUG
			xSerialPrintf_P(PSTR("parola = %i \r\n"), password);
#endif

			//OSGiveSema(&sema_pass);
			//OSEnqueue(password, &pass);
			xTaskNotifyGive( xTaskAlarma);
		}

		//xSerialPrint_P(PSTR("\r\n\n\nTasck Red LEd!\r\n"));
		//PORTD |=  _BV(PORTD3);       // main (red IO_B7) LED on. EtherMega LED on
		//vTaskDelayUntil( &xLastWakeTime, ( 100 / portTICK_PERIOD_MS ) );

		//PORTD &= ~_BV(PORTD3);       // main (red IO_B7) LED off. EtherMega LED off
		//vTaskDelayUntil( &xLastWakeTime, ( 400 / portTICK_PERIOD_MS ) );

//		xSerialPrintf_P(PSTR("RedLED HighWater @ %u\r\n"), uxTaskGetStackHighWaterMark(NULL));
	}
}

/*-----------------------------------------------------------*/
static void TaskAlarma(void *pvParameters)
{
	(void) pvParameters;
	;
	TickType_t xLastWakeTime;
	/* The xLastWakeTime variable needs to be initialised with the current tick
	 count.  Note that this is the only time we access this variable.  From this
	 point on xLastWakeTime is managed automatically by the vTaskDelayUntil()
	 API function. */
	xLastWakeTime = xTaskGetTickCount();

	//DDRD |= _BV(DDD4);

	uint8_t gresit = 0;

	while (1)
	{
		//password = OSDequeue(&pass);

		//Match Password
		vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		contor_s = 0;
		//xSerialPrintf_P(PSTR("parola = %i \r\n"), password);
		//time_sst = GetSeconds();

		if (password == pass_save)
		{
#ifdef DEBUG
			xSerialPrint_P(PSTR("Parola OK! \r\n"));
#endif
			gresit = 0;
			Buzer_PassOK();
			//playFrequency( 523, 150); // ok tone
			if (armat || alarm)
			{
				ALARMOff();
				ARMOff();
#ifdef DEBUG
				xSerialPrint_P(PSTR("Dezarmat! \r\n"));
#endif
				password = 255;
			}
			else
			{
#ifdef DEBUG
				xSerialPrint_P(PSTR("Armeaza! \r\n"));
#endif
				//while (GetSeconds() - time_sst < 15);
				//vTaskDelayUntil( &xLastWakeTime, ( 15000 / portTICK_PERIOD_MS ) );
				while (contor_s < 30) //weit 15s
				{
					playFrequency(5230, 100); // ok tone
					_delay_ms(500);
				}
				//playFrequency( 150, 50); // armare tone
				//OSGiveSema(&sema_senzor);
				ALARMOff();
				//wdt_reset();
				ARMOn();
				password = 255;
			}

		}
		else if (password == 0)
		{

			//If user enters 0000 as password it
			//indicates a request to change password
			playFrequency(1500, 50); // armare tone

#ifdef DEBUG
			xSerialPrint_P(PSTR("Schimba parola \r\n"));
#endif

			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
			if (password == pass_save)
			{
				//Allowed to change password
				//password = 1234;
#ifdef DEBUG
				xSerialPrint_P(PSTR("parola noua: \r\n"));
#endif

				ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

				if (password != 255 && password != 0)
				{
					WritePassToEEPROM(password);

					Buzer_PassOK();
					//playFrequency( 523, 150); // ok tone
					pass_save = password;
#ifdef DEBUG
					xSerialPrint_P(PSTR("Parola schimbata \r\n"));
#endif
				}
				password = 255;

			}
			else
			{
				//Not Allowed to change password
				Buzer_PassNotOK();
				//playFrequency( 2500, 500); // notOK tone
			}

		}
		else if (password != 255)
		{
			++gresit;
			Buzer_PassNotOK();
			//playFrequency( 2500, 500); // notOK tone
			if ((gresit == 4) && armat)
			{
				//ARMOn();
				ALARMOn();
				xSerialPrint_P(PSTR("Sirena pornita \r\n"));
				gresit = 0;
				contor_m = 0;
			}
		}

		/*
		 PORTD |=  _BV(PORTD4);       // main (red PB5) LED on. Arduino LED on
		 vTaskDelayUntil( &xLastWakeTime, ( 10 / portTICK_PERIOD_MS ) );

		 #ifdef portHD44780_LCD
		 lcd_Locate (0, 0);
		 lcd_Printf_P(PSTR("Sys Tick:%7lu"), time(NULL));
		 lcd_Locate (1, 0);
		 lcd_Printf_P(PSTR("Min Heap:%7u"), xPortGetMinimumEverFreeHeapSize() ); // needs heap_4 for this function to succeed.
		 #endif // portHD44780_LCD

		 PORTD &= ~_BV(PORTD4);       // main (red PB5) LED off. Arduino LED off
		 vTaskDelayUntil( &xLastWakeTime, ( 40 / portTICK_PERIOD_MS ) );

		 //		xSerialPrintf_P(PSTR("GreenLED HighWater @ %u\r\n"), uxTaskGetStackHighWaterMark(NULL)); */
	}
}

/*-----------------------------------------------------------*/
static void TaskSenzorR(void *pvParameters)
{
	(void) pvParameters;
	;
	TickType_t xLastWakeTime;
	/* The xLastWakeTime variable needs to be initialised with the current tick
	 count.  Note that this is the only time we access this variable.  From this
	 point on xLastWakeTime is managed automatically by the vTaskDelayUntil()
	 API function. */
	xLastWakeTime = xTaskGetTickCount();
	uint8_t senzor_pull = 1;
	//uint8_t time_mst = 0;
	//DDRD |= ~_BV(DDD4);

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));
		if (armat && !alarm)
		{
			if ((PIND & (1 << PD4)) || (PIND & (1 << PD5)))
			{
				xSerialPrint_P(PSTR("Senzor rapid activat \r\n"));
				ALARMOn();
				contor_m = 0;
				//senzor_pull = 1;
				martor = 1;
				xSerialPrint_P(PSTR("Sirena pornita \r\n"));
			}
		}

		else if (!armat && (PINC & (1 << PC3)) && senzor_pull)
		{
			ALARMOn();
			contor_m = 0;
			senzor_pull = 0;
#ifdef DEBUG
			xSerialPrint_P(PSTR("Sirena pornita BP! \r\n"));
#endif
		}
		else if ((PINC & (1 << PC3)) == 0)
			senzor_pull = 1;

		/*
		 //opresc sirena dupa 2min
		 if (contor_m == 2 && senzor_pull)
		 {
		 ALARMOff();
		 senzor_pull = 0;
		 xSerialPrint_P(PSTR("Sirena oprita \r\n"));
		 }
		 */
#ifdef portHD44780_LCD
		lcd_Locate (0, 0);
		lcd_Printf_P(PSTR("Sys Tick:%7lu"), time(NULL));
		lcd_Locate (1, 0);
		lcd_Printf_P(PSTR("Min Heap:%7u"), xPortGetMinimumEverFreeHeapSize() ); // needs heap_4 for this function to succeed.
#endif // portHD44780_LCD
//		xSerialPrintf_P(PSTR("Senzor rapid HighWater @ %u\r\n"), uxTaskGetStackHighWaterMark(NULL));
	}
}

/*-----------------------------------------------------------*/
static void TaskSenzorL(void *pvParameters)
{
	(void) pvParameters;
	;
	TickType_t xLastWakeTime;
	/* The xLastWakeTime variable needs to be initialised with the current tick
	 count.  Note that this is the only time we access this variable.  From this
	 point on xLastWakeTime is managed automatically by the vTaskDelayUntil()
	 API function. */
	xLastWakeTime = xTaskGetTickCount();

	//DDRD |= _BV(DDD4);
	uint8_t senzor_pull = 0;

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));
		if (armat && !alarm)
		{

			if ((SENZOR_PINS & (1 << SENZOR_PIN))) //(PIND & (1 << PD2)) == 1)
			{
				xSerialPrint_P(PSTR("Senzor intarziat activat \r\n"));
				contor_s = 0;
				for (uint8_t n = 0; n < 15; ++n)
				{
					//PORTC |= (1 << PC3); //buzer on
					//vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));
					//PORTC &= ~(1 << PC3); //buzer off
					playFrequency(1500, 50);
					vTaskDelayUntil(&xLastWakeTime, (90 / portTICK_PERIOD_MS));
				}
				//playFrequency( 1500, 50); // senzor activ tone
				while (contor_s < 12)
					vTaskDelayUntil(&xLastWakeTime, (500 / portTICK_PERIOD_MS));

				if (armat)
				{
					ALARMOn();
					contor_m = 0;
					senzor_pull = 1;
					martor = 2;
					xSerialPrint_P(PSTR("Sirena pornita \r\n"));
				}
			}
		}
#ifdef portHD44780_LCD
		lcd_Locate (0, 0);
		lcd_Printf_P(PSTR("Sys Tick:%7lu"), time(NULL));
		lcd_Locate (1, 0);
		lcd_Printf_P(PSTR("Min Heap:%7u"), xPortGetMinimumEverFreeHeapSize() ); // needs heap_4 for this function to succeed.
#endif // portHD44780_LCD
//		xSerialPrintf_P(PSTR("GreenLED HighWater @ %u\r\n"), uxTaskGetStackHighWaterMark(NULL));
	}
}

static void TaskSemnale(void *pvParameters) // actiouni alarma
{
	(void) pvParameters;
	;
	TickType_t xLastWakeTime;
	/* The xLastWakeTime variable needs to be initialised with the current tick
	 count.  Note that this is the only time we access this variable.  From this
	 point on xLastWakeTime is managed automatically by the vTaskDelayUntil()
	 API function. */
	xLastWakeTime = xTaskGetTickCount();
	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_PERIOD_MS));
		//Lipsa tensiune alimentare
		if (((PIND & (1 << PD6)) == 0) && (contor_s % 15 == 0)) //Lipsa tensiune alimentare
		{
			play(">ARR>ARR>A");
		}

		//senzor activat = led armare trece pe intermitent
		if ((martor == 1) && (contor_s % 2 == 0))
		{
			ARMLED_PORT &= ~(1 << ARMLED_PIN);
			vTaskDelayUntil(&xLastWakeTime, (60 / portTICK_PERIOD_MS));
			ARMLED_PORT |= (1 << ARMLED_PIN);

		}
		else if ((martor == 2) && (contor_s % 2 == 0))
		{
			ARMLED_PORT &= ~(1 << ARMLED_PIN);
			vTaskDelayUntil(&xLastWakeTime, (500 / portTICK_PERIOD_MS));
			ARMLED_PORT |= (1 << ARMLED_PIN);
		}

		if (!armat)
		{
			martor = 0;
			ARMLED_PORT &= ~(1 << ARMLED_PIN);
		}
		//opresc sirena dupa 2min
		if (contor_m == 2 && alarm)
		{
			ALARMOff();
			xSerialPrint_P(PSTR("Sirena oprita \r\n"));
		}

	}
}

/*-----------------------------------------------------------*/
static void SystemInit()
{
	//wdt_disable();
	// turn on the serial port for debugging or for other USART reasons.
	xSerialPort = xSerialPortInitMinimal(USART0, 115200, portSERIAL_BUFFER_TX,
			portSERIAL_BUFFER_RX); //  serial port: WantedBaud, TxQueueLength, RxQueueLength (8n1)

	avrSerialPrint_P(PSTR("\r\n\n\nHello World!\r\n")); // Ok, so we're alive...
	pass_save = ReadPassFromEEPROM();
#ifdef	portHD44780_LCD
	lcd_Init();

	lcd_Print_P(PSTR("Hello World!"));
	lcd_Locate (0, 0);
#endif

	pinSetUp();
	_delay_ms(20);
	Timer1_Init();
	if ((PINC & (1 << PC4)) == 0)
	{
#ifdef DEBUG
		avrSerialPrint_P(PSTR("Scriu parola implicita 1234"));
#endif
		WritePassToEEPROM(1234);
	}

	ARMLED_PORT &= (~(1 << ARMLED_PIN));
	ALARMOff();
	//if(eeprom_read_byte((uint8_t*)3))
	ARMOn();
	//_delay_ms(50);

	//Check if the EEPROM has a valid password or is blank
	if (ReadPassFromEEPROM() == 25755)
	{
		//Password is blank so store a default password
#ifdef DEBUG
		avrSerialPrint_P(PSTR("Scriu parola implicita 1234"));
#endif
		WritePassToEEPROM(1234);
	}

	//wdt_enable(WDTO_1S);
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, portCHAR *pcTaskName)
{

	DDRB |= _BV(DDB4);
	PORTB |= _BV(PORTB4); // main (red PB4) LED on. Mega main LED on and die.
	while (1)
		;
}

/*-----------------------------------------------------------*/

