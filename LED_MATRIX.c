/**
	\file
	\brief
		This is the API for control of a LED MATRIX using Kinetis FRDMK64
	\author Sofía Salazar, Omar Soto
	\date	16/04/2020
	\todo

 */

#include "LED_MATRIX.h"
#include "MK64F12.h"
#include "I2C.h"

/** For testing matrix, these are the address used for it*/
uint8_t matrix_test[] = {0x81, 0x81, 0x70, 0x07, 0x60, 0x68, 0x50, 0x05, 0x40, 0x04, 0x30, 0x15, 0x2F, 0x02, 0x10, 0x01};
void MATRIX_init(void)
{
	/** Start sending the third byte configuration*/
	I2C_tx_rx_mode(TRANSMITTER);
	I2C_start();

	I2C_write_byte(MATRIX_WRITE_ADDRESS);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(FIRST_INIT_VALUE);
	I2C_wait();
	I2C_get_ack();

	I2C_stop();
	/** Start sending the second byte configuration*/
	I2C_start();

	I2C_write_byte(MATRIX_WRITE_ADDRESS);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(SECOND_INIT_VALUE);
	I2C_wait();
	I2C_get_ack();

	I2C_stop();

	/** Start sending the third byte configuration*/
	I2C_start();

	I2C_write_byte(MATRIX_WRITE_ADDRESS);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(THIRD_INIT_VALUE);
	I2C_wait();
	I2C_get_ack();

	I2C_stop();

}

void MATRIX_test(void)
{
	I2C_start();
	/**Start communicating with matrix */
	I2C_write_byte(MATRIX_WRITE_ADDRESS);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(MATRIX_MEM_ADDRESS);
	I2C_wait();
	I2C_get_ack();

	for(uint8_t i = 0; i < 16; i ++  )
	{
		I2C_write_byte(matrix_test[i]);
		I2C_wait();
		I2C_get_ack();
	}

	I2C_stop();
}
