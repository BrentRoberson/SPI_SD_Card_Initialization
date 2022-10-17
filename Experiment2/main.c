/*
 * UART_solution_struct.c
 *
 * Created: 10/5/2021 3:06:09 PM
 * Author : ryoun
 */ 

#include <avr/io.h>
#include "board_struct.h"
#include "GPIO_Outputs.h"
#include "LEDS.h"
#include "UART.h"
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "UART_Print.h"
#include "print_memory.h"
#include "Long_Serial_In.h"
#include "SPI.h"
#include <stdio.h>

const char test_string[15] PROGMEM = "Hello World!\n\r";

int main(void)
{
	SPI_Master_Init(SPI0, 200000);
	Send_Command(24, 0xFF00FF00);
	
	
}
