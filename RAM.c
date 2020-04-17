/**
	\file
	\brief
		This is the driver for write and read functions of a I2C RAM 24AA256
	\author Sofía Salazar, Omar Soto
	\date	16/04/2020
	\todo
 */
#include "I2C.h"
#include "UART.h"
#include "RAM.h"
#include "delay.h"
/** String whom will be returned as a red memory*/
uint8_t g_read_string[] = {FALSE};

#define DELAY 100U
void RAM_write(uint8_t address, uint8_t* data)
{
	I2C_start(I2C_0);
	/**Start comunicating with memory to write*/
	I2C_write_byte(I2C_0, RAM_WRITE_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	I2C_write_byte(I2C_0,RAM_HIGH_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0,address);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	/** This will start a sequential write and stop when memory reaches a end of string*/
	while (*data != '\0') {
		/** Write data on */
		I2C_write_byte(I2C_0, *data);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);
		*data++;
	}
	/** Set a stop bit indicating writing is finally finished*/
	I2C_stop(I2C_0);
}
uint8_t RAM_read_byte(uint8_t address)
{
	I2C_start(I2C_0);
	/**Start comunicating with memory to write and then read*/
	I2C_write_byte(I2C_0,RAM_WRITE_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	I2C_write_byte(I2C_0,RAM_HIGH_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0,address);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	/** Generate a new start for reading*/
	I2C_repeted_start(I2C_0);

	/** Start a read process*/
	I2C_write_byte(I2C_0,RAM_READ_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	/** Changing to receiver mode*/
	I2C_tx_rx_mode(I2C_0,RECEIVER);

	I2C_nack(I2C_0);
	uint8_t data = I2C_read_byte(I2C_0);
	I2C_wait(I2C_0);
	I2C_nack(I2C_0);

	I2C_stop(I2C_0);
	data = I2C_read_byte(I2C_0);

	return data;


}

uint8_t *RAM_read_string(uint8_t address, uint8_t length)
{
	for(uint8_t i = 0; i < length; i++)
	{
		g_read_string[i] = RAM_read_byte(address + i);
	}
	uint8_t *p_memory_string = &g_read_string;
	return p_memory_string;

}
