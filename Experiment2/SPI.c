/*
 * SPI.c
 *
 * Created: 10/16/2022 4:10:59 PM
 *  Author: brrxmc
 */ 
#include "board_struct.h"
#include "GPIO_Outputs.h"

uint8_t error_flag, rcvd_val; 
static uint8_t return_value;
uint8_t no_errors = 0;
uint8_t illegal_command = 'i';
uint8_t clock_rate_error = 'c';
uint8_t SD_timeout_error = 't';
uint8_t SD_comm_error = 'd';

uint8_t rec_values[5];
uint8_t error_value = 0;
//error_value=receive_response(5,rec_values);


//rcvd_val = SPI_Transfer(&SPI0, send_val,&error_flag)
uint8_t SPI_Master_Init(volatile SPI_t *SPI_base, uint32_t clock_rate)
{
	uint16_t divider = (F_CPU/F_DIV)/(clock_rate);
	if (divider<2){
		SPI_base->SPCR = ((1<<SPE)|(1<<MSTR)|(CPOL_BIT<<CPOL)| (CPHA_BIT<<CPHA)|0<<0);
		SPI_base->SPSR = 1;
	}
	else if (divider<4){
		SPI_base->SPCR = ((1<<SPE)|(1<<MSTR)|(CPOL_BIT<<CPOL)| (CPHA_BIT<<CPHA)|0<<0);
		SPI_base->SPSR = 0;
	}
	else if (divider<8){
		SPI_base->SPCR = ((1<<SPE)|(1<<MSTR)|(CPOL_BIT<<CPOL)| (CPHA_BIT<<CPHA)|1<<0);
		SPI_base->SPSR = 1;
	}
	else if (divider<16){
		SPI_base->SPCR = ((1<<SPE)|(1<<MSTR)|(CPOL_BIT<<CPOL)| (CPHA_BIT<<CPHA)|1<<0);
		SPI_base->SPSR = 0;
	}
	else if (divider<32){
		SPI_base->SPCR = ((1<<SPE)|(1<<MSTR)|(CPOL_BIT<<CPOL)| (CPHA_BIT<<CPHA)|2<<0);
		SPI_base->SPSR = 1;
	}
	else if (divider<64){
		SPI_base->SPCR = ((1<<SPE)|(1<<MSTR)|(CPOL_BIT<<CPOL)| (CPHA_BIT<<CPHA)|2<<0);
		SPI_base->SPSR = 0;
	}
	else if (divider<128){
		SPI_base->SPCR = ((1<<SPE)|(1<<MSTR)|(CPOL_BIT<<CPOL)| (CPHA_BIT<<CPHA)|3<<0);
		SPI_base->SPSR = 0;
	}
	else{
		return_value = clock_rate_error;
	}
	if (SPI_base == SPI0){
		GPIO_Output_Set(PB, (1<<5));
		GPIO_Output_Init(PB, (1<<5));
	}
	else{
		GPIO_Output_Set(PB, (1<<3));
		GPIO_Output_Init(PB, (1<<3));
	}
	if(CPOL_BIT == 0){
		GPIO_Output_Clear(PB, (1<<7));
		GPIO_Output_Init(PB, (1<<7));
	}
	else{
		GPIO_Output_Clear(PD, (1<<7));
		GPIO_Output_Init(PD, (1<<7));
	}
	return return_value;
	
}


uint8_t SPI_Transfer( volatile SPI_t * SPI_base, uint8_t send_value)
{
	uint8_t status;
	// First start a transfer by writing send_value to SPDR
	(SPI_base->SPDR) = 0x05;
	// Next wait in a loop until SPIF is set
	do
	{
		status= SPI_base->SPSR;
	}while((status&0x80)==0);
	// Then return the value from SPDR
	return SPI_base->SPDR;
}

uint8_t Send_Command ( uint8_t command, uint32_t argument){
	uint8_t send_value;
	
	if(command<64)
	{
		return_value = no_errors;
		send_value = CMD0|command;
		rcvd_val=SPI_Transfer(SD_SPI_port, send_value);
	}
	else{
		return_value = illegal_command;
		return return_value;
	}
	for(uint8_t index=0;index<4;index++)
	{
		send_value=(uint8_t)(argument>>(24-(index*8)));
		rcvd_val=SPI_Transfer(SD_SPI_port,send_value);
	}
	
	// The final byte to send is determined by the CMD_value.
	if(command==CMD0)
	{
		send_value=CRC7_CMD0;
	}
	else if(command==CMD8)
	{
		send_value=CRC7_CMD8;
	}
	else
	{
		send_value=0x01; // end bit only, CRC7=0
	}
	rcvd_val=SPI_Transfer(SD_SPI_port,send_value);
	// Return the error flag.
	return return_value;
	
}

uint8_t receive_response(uint8_t num_bytes,uint8_t * rec_array){
	return_value = no_errors;
	uint8_t timeout = 0;
	do
	{
		rcvd_val=SPI_Transfer(SD_SPI_port,0xFF); // SPI_Receive
		timeout++;
	}while((rcvd_val==0xFF)&&(timeout!=0));
	// Check for SPI error, timeout error or communication error
	if(timeout==0)
	{
		return_value=SD_timeout_error;
	}
	else if((rcvd_val&0xFE)!=0x00) // 0x00 and 0x01 are good values
	{
		*rec_array=rcvd_val; // return the value to see the error
		return_value=SD_comm_error;
	}
	else// Receive the rest of the bytes (if there are more to receive).
	{
		*rec_array=rcvd_val; // first received value (R1 resp.)
		if(num_bytes>1)
		{
			for(uint8_t index=1;index<num_bytes;index++)
			{
				rcvd_val=SPI_Transfer(SD_SPI_port,0xFF);
				rec_array[index]=rcvd_val;
			}
		}
		rcvd_val=SPI_Transfer(SD_SPI_port,0xFF);
	}
	return return_value;
}
//uint8_t SD_Init(){
