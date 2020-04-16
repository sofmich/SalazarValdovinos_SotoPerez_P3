/**
	\file
	\brief
		This is the driver for write and read functions of a I2C RAM 24AA256
	\author Sofía Salazar, Omar Soto
	\date	16/04/2020
	\todo

 */

#ifndef RAM_H_
#define RAM_H_

#define RAM_WRITE_ADDRESS	0xA0
#define RAM_READ_ADDRESS	0xA1
#define RAM_HIGH_ADDRESS 	0U

/** Low address who indicate the increment of 63 bytes (64) per page on RAM*/
#define PAGE1_LOW_ADDRESS 0x00
#define PAGE2_LOW_ADDRESS 0x40
#define PAGE3_LOW_ADDRESS 0x80
#define PAGE4_LOW_ADDRESS 0xC0


void RAM_write(uint8_t address, uint8_t* data);
uint8_t RAM_read_byte(uint8_t address);
uint8_t  *RAM_read_string(uint8_t address, uint8_t length);

#endif /* RAM_H_ */
