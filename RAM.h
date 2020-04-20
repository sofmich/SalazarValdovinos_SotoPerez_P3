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
#define LOW_MASK 	0x00FF
#define HIGH_MASK	0xFF00

/** RAM High shifter*/
#define HIGH_SHIFT	8U

typedef struct{
	uint8_t address[6];
	uint8_t length[2];
	uint8_t message[99];
}RAM_Data_t;

typedef enum
{
	RAM_write_ = 5,
	RAM_read = 6
} RAM_control_t;

void RAM_write_sequential(uint8_t ram_low, uint8_t ram_high , uint8_t* data);
void RAM_write_byte(uint16_t Address, uint8_t* data);
uint8_t RAM_read_byte(uint16_t Address);
uint8_t  *RAM_read_string(uint8_t address, uint8_t length);

/********************************************************************************************/
/*!
 	 \brief	Allows to initializate the I2C with the optimal baud rate for RAM
 	 \param[in]  void
 	 \return void
 */

void RAM_init(void);

void RAM_prepareTo(uint8_t mode,RAM_Data_t write_data);


#endif /* RAM_H_ */
