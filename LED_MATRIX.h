/**
	\file
	\brief
		This is the API for control of a LED MATRIX HT16K33 using Kinetis FRDMK64
	\author Sofía Salazar, Omar Soto
	\date	16/04/2020
	\todo

 */

#ifndef LED_MATRIX_H_
#define LED_MATRIX_H_


#include "MK64F12.h"

/** Address of the matrix to comunicate through I2C*/
#define MATRIX_WRITE_ADDRESS	0xE0
/** Mem address*/
#define MATRIX_MEM_ADDRESS		0U
/** Initialization constant 1*/
#define FIRST_INIT_VALUE		0x21
#define SECOND_INIT_VALUE		0x81
#define THIRD_INIT_VALUE		0xEF
/** Lenght of MATRIX columns*/
#define MATRIX_LENGTH			16U

typedef enum{
	ON,
	OFF
}dot_status_t;

void MATRIX_init(void);
void MATRIX_show(uint8_t * matrix_cols_values);

uint8_t *MATRIX_char_to_columns(uint8_t char_data);

void MATRIX_off(void);
void MATRIX_dot(dot_status_t MODE);


#endif /* LED_MATRIX_H_ */
