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

/** Address of the matrix to comunicate through I2C*/
#define MATRIX_WRITE_ADDRESS	0x0E
/** Mem address*/
#define MATRIX_MEM_ADDRESS		0U
/** Initialization constant 1*/
#define FIRST_INIT_VALUE		0x21
#define SECOND_INIT_VALUE		0x81
#define THIRD_INIT_VALUE		0xEF


void MATRIX_init(void);
void MATRIX_test(void);


#endif /* LED_MATRIX_H_ */
