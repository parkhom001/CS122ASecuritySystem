/*
Using UART, two ATmegas will communicate with each other
This will be considered the input pinout
Input device: keypad, Motion sensor
*/
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "keypad.h"
#include "usart_ATmega1284.h"
#include "timer.h"

//input Vals
//motion = 0x00 for detected
unsigned char btn;
unsigned char motionSensor;
unsigned char beamSensor;
unsigned char myData;
unsigned char keypad[4];

//check state vals
unsigned char pinEntered;
unsigned char motionOutput;
unsigned char beamOutput;
unsigned char correctPin;

enum motionstate {motionInit, motionOn} motionState;
void motionSensorfunc()
{
	motionSensor = (PINA & 0x01);
	
	switch(motionState)
	{
		case motionInit:
		_delay_ms(100);
		if (!motionSensor) // at 0x00, sensor is triggered
			motionSensor = motionOn;
		else
			motionState = motionInit;
		break;
		case motionOn:
		break;
		default: break;
	}
	switch (motionState)
	{
		case motionInit:
//		motionOutput = 0x02;
		break;
		case motionOn:
		motionOutput = 0x02;
		break;
	}
}

enum sensorstate {beamInit, beamActive} sensorState;
void sensorFunc()
{
	beamSensor = (PINA & 0x02);
	
	switch(sensorState)
	{
		case beamInit:
		if (beamSensor == 0x00)
			sensorState = beamActive;
		else 
			sensorState = beamInit;
		break;
		case beamActive:
		break;
	}	//transition state
	switch(sensorState)
	{
		case beamInit:
//		beamSensor = 0x00;
//		beamOutput = 0x00;
		break;
		case beamActive:
		beamOutput = 0x01;
		break;
		
	}
}

int main(void)
{
	initUSART(0);	
	initUSART(1);
	
	motionSensor = 0;
	beamSensor = 0;
	myData = 0;
	pinEntered = 0;
	unsigned int globalPeriod = 50;
	int count = 0;
	//inputs
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;
	//keypad
//	DDRB = 0xF0; PORTB = 0x0F;
	
//	TimerSet(globalPeriod);
//	TimerOn();
	sensorState = beamInit;
	motionState = motionInit;
	
	
	while (1)
	{

//		pinEntered = 0x01;
//		correctPin = 0x01;
		// for output atmega, defaults to pinentered not correct.
		while(1)
		{
			if (USART_IsSendReady(0))
			{	
		
				btn = (PINC & 0x01);
				if (!btn)
				{
					USART_Send(0x04,0);
					motionOutput = 0;
					motionSensor = 0;
					beamOutput = 0;
					beamSensor = 0;
					sensorState = beamInit;
					motionState = motionInit;
					btn = 0x00;
					myData = 0x00;
				}
				motionSensorfunc();
				sensorFunc();
		//		motionOutput = 0x02;
				myData = motionOutput + beamOutput;
	
				if (myData != 0x00) 
					{
					USART_Send(myData,0);	
					}
			break;
			}
			if (USART_IsSendReady(1))
			{
				if (myData)
				{
					USART_Send(0x01,1);
				}
			}
		}
	}
}
