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
#include "stdio.h"

#define DELAY_RAM 800U

/** String whom will be returned as a red memory*/
uint8_t g_read_string[] = {FALSE};


/*baud rate of 115200 KBPS 400K**/
static const i2c_baud_rate_t I2C_RAM_baud_rate = {2, 3};

static const i2c_config_struct_t RAM_config =
{
		I2C_0, /** Channel*/
		21000000, /** System clock*/
		I2C_RAM_baud_rate.mult, /** Multiplier*/
		I2C_RAM_baud_rate.icr, /** Multiplier*/
};

void RAM_init(void)
{
	/* Configure the selected I2C channel with a Baud Rate of 115200. */
	I2C_init(&RAM_config);
}


void RAM_write_sequential(uint8_t ram_low, uint8_t ram_high , uint8_t* data)
{
	uint8_t ackError;

	I2C_start(I2C_0);
	/**Start comunicating with memory to write*/
	I2C_write_byte(I2C_0, RAM_WRITE_ADDRESS);
	I2C_wait(I2C_0);
	ackError = I2C_get_ack(I2C_0);

	/** Show error message when acknowledge is TRUE*/
	if(TRUE == ackError)
	{
		SYSTEM_I2C_fail(RAM_write_);
	}
	else{
		I2C_write_byte(I2C_0, ram_high);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);

		I2C_write_byte(I2C_0, ram_low);
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
}

void RAM_write_byte(uint16_t Address, uint8_t* data)
{
	uint8_t ackError = FALSE;

	uint8_t ram_low = Address & LOW_MASK;
	uint8_t ram_high = Address >> HIGH_SHIFT;
	ram_high &= LOW_MASK;

	printf("\nESCRITURA LOW 0x%x HIGH 0x%x DATA %c", ram_low, ram_high, *data);

	I2C_start(I2C_0);
	/**Start comunicating with memory to write*/
	I2C_write_byte(I2C_0, RAM_WRITE_ADDRESS);
	I2C_wait(I2C_0);
	ackError = I2C_get_ack(I2C_0);

	/** Show error message when acknowledge is TRUE*/
	if(TRUE == ackError)
	{
		SYSTEM_I2C_fail(RAM_write_);
	}
	else{
		I2C_write_byte(I2C_0, ram_high);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);

		I2C_write_byte(I2C_0, ram_low);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);

		I2C_write_byte(I2C_0, *data);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);

		/** Set a stop bit indicating writing is finally finished*/
		I2C_stop(I2C_0);
	}

}
uint8_t RAM_read_byte(uint16_t Address)
{
	uint8_t ackError = FALSE;

	uint8_t ram_low = Address & LOW_MASK;
	uint8_t ram_high = Address >> HIGH_SHIFT;
	ram_high &= LOW_MASK;

	printf("\nLECTURA \n LOW 0x%x HIGH 0x%x DATA  ", ram_low, ram_high);

	I2C_start(I2C_0);
	/**Start comunicating with memory to write and then read*/
	I2C_write_byte(I2C_0,RAM_WRITE_ADDRESS);
	I2C_wait(I2C_0);
	ackError = I2C_get_ack(I2C_0);

	/** Show error message when acknowledge is TRUE*/
	if(TRUE == ackError)
	{
		SYSTEM_I2C_fail(RAM_write_);
	}
	else
	{
		I2C_write_byte(I2C_0,ram_high);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);

		I2C_write_byte(I2C_0,ram_low);
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
		printf(" %c", data);
		return data;

	}
}

void RAM_prepareTo(uint8_t mode, RAM_Data_t write_data)
{
	uint8_t highAddress = FALSE;
	uint8_t lowAddress = FALSE;
	uint8_t length = FALSE;
	uint16_t Address = FALSE;

	/** From the array it comes on [0]x[A][B][4][5]*/
	/** Prepare from ASCII to numeric values*/
	for(uint8_t i = 2; i<6; i++)
	{
		if(write_data.address[i] == 'a' || write_data.address[i] == 'A' ) write_data.address[i] = 0x0A;
		else if(write_data.address[i] == 'b' || write_data.address[i] == 'B' ) write_data.address[i] = 0x0B;
		else if(write_data.address[i] == 'c' || write_data.address[i] == 'C' ) write_data.address[i] = 0x0C;
		else if(write_data.address[i] == 'd' || write_data.address[i] == 'D' ) write_data.address[i] = 0x0D;
		else if(write_data.address[i] == 'e' || write_data.address[i] == 'E' ) write_data.address[i] = 0x0E;
		else if(write_data.address[i] == 'f' || write_data.address[i] == 'F' ) write_data.address[i] = 0x0F;
		else write_data.address[i]  = write_data.address[i] - 48 ;
	}



	Address = (write_data.address[2] << 12 | write_data.address[3] << 8 )
			| (write_data.address[4] << 4 | write_data.address[5]);
	/** From the array it comes on [cents][tens][units] three digits*/
	length = (write_data.length[0] * 10)+(write_data.length[1]);
	printf("\nADDRESS: 0x%x\n", Address);

	switch(mode)
	{
	case(RAM_write_):
	{
		for(uint8_t i =0; i < length; i++)
		{
			/** And now we should just care to write the message */
			RAM_write_byte(Address, &(write_data.message[i]));
			delay(DELAY_RAM);
			Address++;
		}
	}
	break;
	case(RAM_read):
	{

		uint8_t data = FALSE;
		for(uint8_t i =0; i < length ; i++)
		{
			data = RAM_read_byte(Address);
			delay(DELAY_RAM);
			printf("\n%c", data);
			Address++;
		}

	}
	break;
	default:
		break;
	}
}

