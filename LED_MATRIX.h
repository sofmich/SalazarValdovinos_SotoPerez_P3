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


/********************************************************************************************/
/*!
 	 \brief	 Send the commands to comunicate thorugh I2C
 	 \param[in]  void
 	 \return void
 */
void MATRIX_init(void);
/********************************************************************************************/
/*!
 	 \brief	 Go through the values to print on matrix
 	 \param[in]  pointer to data to print
 	 \return void
 */
void MATRIX_show(uint8_t * matrix_cols_values);
/********************************************************************************************/
/*!
 	 \brief	 Converts caracters or numbers to its value on matrix
 	 \param[in]  pointer to data to print
 	 \return void
 */
uint8_t *MATRIX_char_to_columns(uint8_t char_data);
/********************************************************************************************/
/*!
 	 \brief	 Shuts down all leds on matrix
 	 \param[in]
 	 \return void
 */
void MATRIX_off(void);
/********************************************************************************************/
/*!
 	 \brief	 Get the numeric data time and maps it to matrix values
 	 \param[in] 	void
 	 \return void
 */
void MATRIX_show_time(void);


#endif /* LED_MATRIX_H_ */
