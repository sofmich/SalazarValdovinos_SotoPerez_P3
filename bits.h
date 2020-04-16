/**
	\file bits.h
	\brief
		This is the header file to define all bits constants for each port.
		All bits implementations are declared in here.
	\author Sofía Salazar, Omar Soto
	\date	February 28, 2020
 */

#ifndef BITS_H_
#define BITS_H_

/*! This definition is as a general definitions to bits in register or pins in the microcontroller.*/
typedef enum {bit_0,  /*!< Bit 0 */
			  bit_1,  /*!< Bit 1 */
			  bit_2,  /*!< Bit 2 */
			  bit_3,  /*!< Bit 3 */
			  bit_4,  /*!< Bit 4 */
			  bit_5,  /*!< Bit 5 */
			  bit_6,  /*!< Bit 6 */
			  bit_7,  /*!< Bit 7 */
	          bit_8,  /*!< Bit 8 */
	          bit_9,  /*!< Bit 9 */
			  bit_10, /*!< Bit 10 */
			  bit_11, /*!< Bit 11 */
			  bit_12, /*!< Bit 12 */
			  bit_13, /*!< Bit 13 */
			  bit_14, /*!< Bit 14 */
			  bit_15, /*!< Bit 15 */
			  bit_16, /*!< Bit 16 */
			  bit_17, /*!< Bit 17 */
			  bit_18, /*!< Bit 18 */
			  bit_19, /*!< Bit 19 */
			  bit_20, /*!< Bit 20 */
			  bit_21, /*!< Bit 21 */
			  bit_22, /*!< Bit 22 */
			  bit_23, /*!< Bit 23 */
			  bit_24, /*!< Bit 24 */
			  bit_25, /*!< Bit 25 */
			  bit_26, /*!< Bit 26 */
			  bit_27, /*!< Bit 27 */
			  bit_28, /*!< Bit 28 */
			  bit_29, /*!< Bit 29 */
	          bit_30, /*!< Bit 30 */
			  bit_31  /*!< Bit 31 */
			} bit_t;
/*! This definition is a general definition of bits to use number of bit as HEX value*/
typedef enum{
	bit_in_0 = 		0x0,
	bit_in_1 = 		0x02,
	bit_in_2 = 		0x04,
	bit_in_3 = 		0x08,
	bit_in_4 = 		0x0010,
	bit_in_5 = 		0x0020,
	bit_in_6 = 		0x0040,
	bit_in_7 = 		0x0080,
	bit_in_8 = 		0x0100,
	bit_in_9 = 		0x0200,
	bit_in_10 = 	0x0400,
	bit_in_11 = 	0x0800,
	bit_in_12 = 	0x001000,
	bit_in_13 = 	0x002000,
	bit_in_14 = 	0x004000,
	bit_in_15 = 	0x008000,
	bit_in_16 = 	0x010000,
	bit_in_17 = 	0x020000,
	bit_in_18 = 	0x040000,
	bit_in_19 = 	0x080000,
	bit_in_20 = 	0x00100000,
	bit_in_21 = 	0x00200000,
	bit_in_22 = 	0x00400000,
	bit_in_23 = 	0x00800000,
	bit_in_24 = 	0x01000000,
	bit_in_25 = 	0x02000000,
	bit_in_26 = 	0x04000000,
	bit_in_27 = 	0x08000000,
	bit_in_28 = 	0x010000000,
	bit_in_29 = 	0x020000000,
	bit_in_30 = 	0x040000000,
	bit_in_31 = 	0x080000000
} bit_input_t;



typedef enum {FALSE, TRUE} boolean_t;
/*! This definition is as a general definitions to bits turn-on or turn-off any bit*/
typedef enum {BIT_OFF, BIT_ON} bit_on_off_t;

#endif /* BITS_H_ */
