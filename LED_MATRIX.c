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
#include "system_status.h"
#include "delay.h"
#include "GPIO.h"
#include "stdio.h"

#define DELAY 100U
/** Global CONSTANS to map decoder of hour digits*/
const uint8_t decoder_time_0[] = { 0x00, 0xFE, 0x82, 0xFE}; /*Zero digit to init*/
const uint8_t decoder_time_1[] = { 0x00, 0x00, 0xFE, 0x00 }; /*1 digit to init*/
const uint8_t decoder_time_2[] = { 0x00, 0x9E, 0x92, 0xF2}; /*2 digit to init*/
const uint8_t decoder_time_3[] = { 0x00, 0x92, 0x92, 0xFE }; /*3 digit to init*/
const uint8_t decoder_time_4[] = { 0x00, 0xF0, 0x10, 0xFE }; /*4 digit to init*/
const uint8_t decoder_time_5[] = { 0x00, 0xF2, 0x92, 0x9E }; /*5 digit to init*/
const uint8_t decoder_time_6[] = { 0x00, 0xFE, 0x92, 0x1E }; /*6 digit to init*/
const uint8_t decoder_time_7[] = { 0x00, 0x80, 0x90, 0xFE }; /*7 digit to init*/
const uint8_t decoder_time_8[] = { 0x00, 0xFE, 0x92, 0xFE }; /*8 digit to init*/
const uint8_t decoder_time_9[] = { 0x00, 0xF0, 0x90, 0xFE }; /*9 digit to init*/

/** Variable to set the actual status of error*/
uint8_t g_Status_MATRIX = FALSE;

/** Matrix useful to save values to print*/
uint8_t g_values_to_print[MATRIX_LENGTH] = {FALSE};

/** Pointer to function to execute function from system_status*/
void (*ptr_SystemError_Matrix)(uint8_t)=SYSTEM_I2C_fail;

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
	uint8_t ackError = FALSE;

	/** Start sending the third byte configuration*/
	I2C_tx_rx_mode(I2C_0, TRANSMITTER);
	I2C_start(I2C_0);

	I2C_write_byte(I2C_0,MATRIX_WRITE_ADDRESS);
	I2C_wait(I2C_0);
	ackError = I2C_get_ack(I2C_0);

	if(TRUE == ackError )
	{
		ptr_SystemError_Matrix(g_Status_MATRIX);
		I2C_stop(I2C_0);
	}
	else{
		I2C_write_byte(I2C_0,MATRIX_MEM_ADDRESS);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);

		/** Wryte bytes of matrix 1,2 column 1*/
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


}

uint8_t *MATRIX_char_to_columns(uint8_t char_data)
{
	uint8_t *p_data = FALSE;
	switch(char_data)
	{
	case(0):
		p_data = decoder_time_0;
			break;
	case(1):
		p_data = decoder_time_1;
			break;
	case(2):
		p_data = decoder_time_2;
				break;
	case(3):
		p_data = decoder_time_3;
				break;
	case(4):
		p_data = decoder_time_4;
				break;
	case(5):
		p_data = decoder_time_5;
				break;
	case(6):
		p_data = decoder_time_6;
				break;
	case(7):
		p_data = decoder_time_7;
				break;
	case(8):
		p_data = decoder_time_8;
				break;
	case(9):
		p_data = decoder_time_9;
				break;
	case 'a':
		return (0x003F7FC8C87F3F00);
		break;
	case 'A':
		return (0x003F7FC8C87F3F00);
		break;

	case 'b':
		return (0x007F7F49493F3600);
		break;
	case 'B':
		return (0x007F7F49493F3600);
		break;

	case 'c':
		return (0x003E7F4141632200);
		break;
	case 'C':
		return (0x003E7F4141632200);
		break;

	case 'd':
		return (0x007F7F41417F3E00);
		break;
	case 'D':
		return (0x007F7F41417F3E00);
		break;

	case 'e':
		return (0x007F7F4949494100);
		break;
	case 'E':
		return (0x007F7F4949494100);
		break;

	case 'f':
		return (0x007F7F4848484000);
		break;
	case 'F':
		return (0x007F7F4848484000);
		break;

	case 'g':
		return (0x003E7F4145672600);
		break;
	case 'G':
		return (0x003E7F4145672600);
		break;

	case 'h':
		return (0x007F7F08087F7F00);
		break;
	case 'H':
		return (0x007F7F08087F7F00);
		break;

	case 'i':
		return (0x0000417F7F410000);
		break;
	case 'I':
		return (0x0000417F7F410000);
		break;

	case 'j':
		return (0x000607417F7E4000);
		break;
	case 'J':
		return (0x000607417F7E4000);
		break;

	case 'k':
		return (0x007F7F1C36634100);
		break;
	case 'K':
		return (0x007F7F1C36634100);
		break;

	case 'l':
		return (0x007F7F0101010100);
		break;
	case 'L':
		return (0x007F7F0101010100);
		break;

	case 'm':
		return (0x007F7F3018307F7F);
		break;
	case 'M':
		return (0x007F7F3018307F7F);
		break;

	case 'n':
		return (0x007F7F30180C7F7F);
		break;
	case 'N':
		return (0x007F7F30180C7F7F);
		break;

	case 'o':
		return (0x003E7F41417F3E00);
		break;
	case 'O':
		return (0x003E7F41417F3E00);
		break;

	case 'p':
		return (0x007F7F44447C3800);
		break;
	case 'P':
		return (0x007F7F44447C3800);
		break;

	case 'q':
		return (0x003C7E42427F3D00);
		break;
	case 'Q':
		return (0x003C7E42427F3D00);
		break;

	case 'r':
		return (0x007F7F4C4E7B3100);
		break;
	case 'R':
		return (0x007F7F4C4E7B3100);
		break;

	case 's':
		return (0x00327B49496F2600);
		break;
	case 'S':
		return (0x00327B49496F2600);
		break;

	case 't':
		return (0x0060607F7F606000);
		break;
	case 'T':
		return (0x0060607F7F606000);
		break;

	case 'u':
		return (0x007E7F01017F7F00);
		break;
	case 'U':
		return (0x007E7F01017F7F00);
		break;

	case 'v':
		return (0x00FCFE0303FEFC00);
		break;
	case 'V':
		return (0x00FCFE0303FEFC00);
		break;

	case 'w':
		return (0x007F7F060C067F7F);
		break;
	case 'W':
		return (0x007F7F060C067F7F);
		break;

	case 'x':
		return (0x0060771C081C7763);
		break;
	case 'X':
		return (0x0060771C081C7763);
		break;

	case 'y':
		return (0x0070780F0F787000);
		break;
	case 'Y':
		return (0x0070780F0F787000);
		break;

	case 'z':
		return (0x0043474D59716100);
		break;
	case 'Z':
		return (0x0043474D59716100);
		break;

	case ' ':
		return (0x8000000000000000);
		break;
	case '  ':
		return (0x8000000000000000);
		break;

	default:
		return 0;
		break;
	}
	return p_data;
}

void MATRIX_show_time(void)
{
	uint8_t auxDot = GPIO_read_pin(GPIO_D, bit_0);
	/** Now the time is in format cents and units for each component*/
	time_format_t actualTime = GetGlobalTime();
	g_Status_MATRIX = READ_TIME;
	uint8_t *(p_valuesToPrint)[4] = {FALSE};
	/** Values to print are saved on a matrix through the content of a pointer*/
	p_valuesToPrint[0] = (MATRIX_char_to_columns(actualTime.tens_hours));
	p_valuesToPrint[1] = (MATRIX_char_to_columns(actualTime.units_hours));
	p_valuesToPrint[2] = (MATRIX_char_to_columns(actualTime.tens_minutes));
	p_valuesToPrint[3] = (MATRIX_char_to_columns(actualTime.units_minutes));

    for(uint8_t i = 0; i<4; i++)
    {
    	g_values_to_print[i] 	= *(p_valuesToPrint[0]+i);
    	g_values_to_print[i+4] 	= *(p_valuesToPrint[1]+i);
    	g_values_to_print[i+8] 	= *(p_valuesToPrint[2]+i);
    	g_values_to_print[i+12] = *(p_valuesToPrint[3]+i);
    }

    g_values_to_print[0] = 0x01;
    MATRIX_show(g_values_to_print);
    uint8_t auxDot_loop = auxDot;
    do{
    	auxDot_loop = GPIO_read_pin(GPIO_D, bit_0);
    } while(auxDot_loop == auxDot);
    g_values_to_print[0] = 0x00;
	MATRIX_show(g_values_to_print);

}


void MATRIX_off(void)
{
	/** Create an array to set all leds OFF*/
	uint8_t clean_MATRIX[16] = {FALSE};
	MATRIX_show(clean_MATRIX);
}
