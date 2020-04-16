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

#define DELAY 100U
void RAM_write(uint8_t address, uint8_t* data)
{
	I2C_start(I2C_O);
	/**Start comunicating with memory to write*/
	I2C_write_byte(I2C_0, RAM_WRITE_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	I2C_write_byte(I2C_0, RAM_HIGH_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0, address);
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
	I2C_start(I2C_O);
	/**Start comunicating with memory to write and then read*/
	I2C_write_byte(I2C_0, RAM_WRITE_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	I2C_write_byte(I2C_0, RAM_HIGH_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0, address);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	/** Generate a new start for reading*/
	I2C_repeted_start(I2C_0); //Generating a new start for Reading

	/** Start a read process*/
	I2C_write_byte(I2C_0, RAM_READ_ADRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	/** Changing to receiver mode*/
	I2C_tx_rx_mode(I2C_0, RECEIVER_MODE);

	I2C_nack();
	uint8_t data = I2C_read_byte();
	I2C_wait(I2C_0);
	I2C_nack(I2C_0);

	I2C_stop(I2C_0);
	data = I2C_read_byte(I2C_0);

	return cadena;


}

uint8_t *RAM_read_string(uint8_t address, uint8_t length)
{

}
