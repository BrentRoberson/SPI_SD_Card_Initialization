/*
 * SPI.h
 *
 * Created: 10/16/2022 4:08:58 PM
 *  Author: brrxmc
 */ 


#ifndef SPI_H_
#define SPI_H_

uint8_t SPI_Master_Init(volatile SPI_t *SPI_base, uint32_t clock_rate); 
uint8_t SPI_Transfer(volatile SPI_t *SPI_base, uint8_t send_value);
uint8_t Send_Command ( uint8_t command, uint32_t argument);
uint8_t receive_response(uint8_t num_bytes, uint8_t * rec_array);
#endif /* SPI_H_ */