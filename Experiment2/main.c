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

uint8_t array[10] = {};
uint8_t *array_ptr = array;

int main(void)
{
	
	UART_Init(UART1, 9600);
	SPI_Master_Init(SPI0, 400000);
	Send_Command(24, 0xFF00FF00);
	receive_response(5,array_ptr);
	UART_Transmit(UART1, array_ptr[0]);
	set_error_status();
	

	
	
}
