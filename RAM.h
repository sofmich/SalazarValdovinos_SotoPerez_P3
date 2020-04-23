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

/** RAM write address*/
#define RAM_WRITE_ADDRESS	0xA0
/** RAM read address*/
#define RAM_READ_ADDRESS	0xA1
/** Constant that represents when it is an empty space*/
#define TRASH_Y 			255U
/** Low address to mask and send over I2C*/
#define LOW_MASK 	0x00FF
/** High address to mask and send over I2C*/
#define HIGH_MASK	0xFF00
/** RAM High shifter*/
#define HIGH_SHIFT	8U
/** Structure to work though modules*/
typedef struct{
	uint8_t address[6];
	uint8_t length[2];
	uint8_t message[99];
}RAM_Data_t;
typedef struct
{
	uint8_t (p_message1)[50];
	uint8_t (p_message2)[50];
	uint8_t (p_message3)[50];

}first_messages_t;
/** Status to indicate erorrs*/
typedef enum
{
	RAM_write_ = 5,
	RAM_read = 6
} RAM_control_t;
/********************************************************************************************/
/*!
 	 \brief	Reads the first three messages on RAM
 	 \param[in]
 	 \return structure with pointers to messages
 */
first_messages_t readFirstMessages(void);
void RAM_write_sequential(uint8_t ram_low, uint8_t ram_high , uint8_t* data);
/********************************************************************************************/
/*!
 	 \brief	Writes a byte on RAM
 	 \param[in]  address
 	 \param[in]  pointer to data
 	 \return void
 */
void RAM_write_byte(uint16_t Address, uint8_t* data);
/********************************************************************************************/
/*!
 	 \brief	Reads a byte on RAM
 	 \param[in]  address
 	 \return data of 8 bits
 */
uint8_t RAM_read_byte(uint16_t Address);
/********************************************************************************************/
/*!
 	 \brief	Could be used for sequential read
 	 \param[in]  address
 	 \return data of 8 bits
 */
uint8_t  *RAM_read_string(uint8_t address, uint8_t length);

/********************************************************************************************/
/*!
 	 \brief	Allows to initializate the I2C with the optimal baud rate for RAM
 	 \param[in]  void
 	 \return void
 */
void RAM_init(void);
/********************************************************************************************/
/*!
 	 \brief	 Prepares the data from structure to calculate length, values and address
 	 \param[in]  active mode of RAM, wheter it is read or write
 	 \return void
 */
void RAM_prepareTo(uint8_t mode,RAM_Data_t write_data);


#endif /* RAM_H_ */
