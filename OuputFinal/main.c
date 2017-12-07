/* Nokia 5110 LCD AVR Library example
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 */

#include <avr/io.h>
#include <util/delay.h>
#include "usart_ATmega1284.h"
#include "timer.h"

#include "nokia5110.h"

//unsigned char correctPin = 0x00;
unsigned char receivedData = 0x00;

void receiveData()
{
	if (USART_HasReceived(0))	
	{
		receivedData = USART_Receive(0);
		USART_Flush(0);
	}
}
int main(void)
{
    nokia_lcd_init();
    nokia_lcd_clear();
	initUSART(0);
	initUSART(1);

    while(1) 
	{		 
		_delay_ms(1000);
//		while(1)
//		{
			if (USART_HasReceived(0))
			{
				
			receivedData = USART_Receive(0);
			USART_Flush(0);
			break;
			}
//		}
		
//		nokia_lcd_write_string("Checking sensors",1);
		_delay_ms(1000);
		nokia_lcd_clear();
		if (receivedData == 0x00)
		{
				nokia_lcd_write_string("Loading", 1);
				nokia_lcd_clear();
		}
		else if (receivedData == 0x02)
			nokia_lcd_write_string("Motion sensor detected", 1);
		else if (receivedData == 0x01)
		{
			nokia_lcd_write_string("Break beam sensor detected", 1);
		}
		else if (receivedData == 0x03)
		{
			nokia_lcd_write_string("Both sensors  were tripped", 1);
		}
		else if (receivedData == 0x04)
		{
			nokia_lcd_write_string("Reinitializing Security system", 1);
			receivedData = 0x00;	
			nokia_lcd_clear;
		}
		nokia_lcd_render();	
		_delay_ms(100);
//	   while(!TimerFlag);
//	   TimerFlag = 0;
    
	}
}
