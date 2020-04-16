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
	I2C_tx_rx_mode(I2C_0, TRANSMITTER);
	I2C_start(I2C_0);

	I2C_write_byte(I2C_0,MATRIX_WRITE_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0,FIRST_INIT_VALUE);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_stop(I2C_0);
	/** Start sending the second byte configuration*/
	I2C_start(I2C_0);

	I2C_write_byte(I2C_0,MATRIX_WRITE_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0,SECOND_INIT_VALUE);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_stop(I2C_0);

	/** Start sending the third byte configuration*/
	I2C_start(I2C_0);

	I2C_write_byte(I2C_0,MATRIX_WRITE_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0,THIRD_INIT_VALUE);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_stop(I2C_0);

}

void MATRIX_test(void)
{
	I2C_start(I2C_0);
	/**Start communicating with matrix */
	I2C_write_byte(I2C_0,MATRIX_WRITE_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0,MATRIX_MEM_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	for(uint8_t i = 0; i < 16; i ++  )
	{
		I2C_write_byte(I2C_0,matrix_test[i]);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);
	}

	I2C_stop(I2C_0);
}
