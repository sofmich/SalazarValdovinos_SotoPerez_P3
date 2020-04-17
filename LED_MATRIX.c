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
#include "MCP7940.h"

#define DELAY 100U
/** Global CONSTANS to map decoder of hour digits*/
const uint8_t decoder_time_0[] = { 0x00, 0xFE, 0x82, 0xFE}; /*Zero digit to init*/
const uint8_t decoder_time_1[] = { 0x00, 0x00, 0xFF, 0x00 }; /*1 digit to init*/
const uint8_t decoder_time_2[] = { 0x00, 0x9E, 0x92, 0xF2}; /*2 digit to init*/
const uint8_t decoder_time_3[] = { 0x00, 0x92, 0x92, 0xFE }; /*3 digit to init*/
const uint8_t decoder_time_4[] = { 0x00, 0xF0, 0x10, 0xFF }; /*4 digit to init*/
const uint8_t decoder_time_5[] = { 0x00, 0xF9, 0x89, 0x8F }; /*5 digit to init*/
const uint8_t decoder_time_6[] = { 0x00, 0xFE, 0x8A, 0x1E }; /*6 digit to init*/
const uint8_t decoder_time_7[] = { 0x00, 0x80, 0x90, 0xFF }; /*7 digit to init*/
const uint8_t decoder_time_8[] = { 0x00, 0xFE, 0x8A, 0xFE }; /*8 digit to init*/
const uint8_t decoder_time_9[] = { 0x00, 0xF0, 0x90, 0xFF }; /*9 digit to init*/



/** Matrix useful to save values to print*/
uint8_t g_values_to_print[MATRIX_LENGTH] = {FALSE};

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


void MATRIX_show(uint8_t *matrix_cols_values)
{
	/** Start sending the third byte configuration*/
	I2C_tx_rx_mode(I2C_0, TRANSMITTER);
	I2C_start(I2C_0);

	I2C_write_byte(I2C_0,MATRIX_WRITE_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0,MATRIX_MEM_ADDRESS);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	/** Wryte bytes of matrix column 1*/
	I2C_write_byte(I2C_0, *(matrix_cols_values));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	I2C_write_byte(I2C_0, *(matrix_cols_values + 8));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	/** Wryte bytes of matrix column 2*/
	I2C_write_byte(I2C_0, *(matrix_cols_values+1));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	I2C_write_byte(I2C_0, *(matrix_cols_values + 9));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	/** Wryte bytes of matrix column 3*/
	I2C_write_byte(I2C_0, *(matrix_cols_values+2));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	I2C_write_byte(I2C_0, *(matrix_cols_values + 10));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	/** Wryte bytes of matrix column 4*/
	I2C_write_byte(I2C_0, *(matrix_cols_values+3));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	I2C_write_byte(I2C_0, *(matrix_cols_values + 11));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	/** Wryte bytes of matrix column 5*/
	I2C_write_byte(I2C_0, *(matrix_cols_values+4));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	I2C_write_byte(I2C_0, *(matrix_cols_values + 12));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	/** Wryte bytes of matrix column 6*/
	I2C_write_byte(I2C_0, *(matrix_cols_values + 5));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	I2C_write_byte(I2C_0, *(matrix_cols_values + 13));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	/** Wryte bytes of matrix column 7*/
	I2C_write_byte(I2C_0, *(matrix_cols_values + 6));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	I2C_write_byte(I2C_0, *(matrix_cols_values + 14));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	/** Wryte bytes of matrix column 8*/
	I2C_write_byte(I2C_0, *(matrix_cols_values + 7));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);


	I2C_write_byte(I2C_0, *(matrix_cols_values + 15));
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_stop(I2C_0);
}

uint8_t *MATRIX_char_to_columns(uint8_t char_data)
{
	uint8_t *p_data = FALSE;
	switch(char_data)
	{
	case('0'):
		p_data = decoder_time_0;
			break;
	case('1'):
		p_data = decoder_time_1;
			break;
	case('2'):
		p_data = decoder_time_2;
				break;
	case('3'):
		p_data = decoder_time_3;
				break;
	case('4'):
		p_data = decoder_time_4;
				break;
	case('5'):
		p_data = decoder_time_5;
				break;
	case('6'):
		p_data = decoder_time_6;
				break;
	case('7'):
		p_data = decoder_time_7;
				break;
	case('8'):
		p_data = decoder_time_8;
				break;
	case('9'):
		p_data = decoder_time_9;
				break;
	default:
				break;

	}
	return p_data;
}

void MATRIX_show_time(void)
{
	/** Now the time is in format cents and units for each component*/
	time_format_t actualTime = GetGlobalTime();
	/** Values to print are saved on a matrix through the content of a pointer*/
	g_values_to_print[0] = *(MATRIX_char_to_columns(actualTime.tens_hours));
	g_values_to_print[4] = *(MATRIX_char_to_columns(actualTime.units_hours));
	g_values_to_print[8] = *(MATRIX_char_to_columns(actualTime.tens_minutes));
    g_values_to_print[12] = *(MATRIX_char_to_columns(actualTime.tens_minutes));
    MATRIX_show(g_values_to_print);
    delay(DELAY);
    MATRIX_dot(ON);

}
void MATRIX_dot(dot_status_t MODE)
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


	switch(MODE)
	{
	case('ON'):
	{
		/* Write on the matrix 0 column 0, write dot*/
		I2C_write_byte( I2C_0, 0x01);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);

	}
			break;
	case('OFF'):
	{
		/* Write on the matrix 0 column 0, turn off dot*/
		I2C_write_byte(I2C_0, 0x00);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);

	}
	break;
	}

	I2C_stop(I2C_0);

}


void MATRIX_off(void)
{
	/** Create an array to set all leds OFF*/
	uint8_t clean_MATRIX[16] = {FALSE};
	MATRIX_show(clean_MATRIX);
	MATRIX_dot(OFF);
}
