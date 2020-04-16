/**
	\file
	\brief
		This is the header file for the UART device driver.
		It contains the macros and function definition.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	05/03/2019
	\todo
		To implement all needed functions
 */
#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "bits.h"

/**
 * \brief A mail box type definition for serial port
 */
typedef struct{
	uint8_t flag; 		/** Flag to indicate that there is new data*/
	uint8_t mailBox;    /** it contains the received data*/
} uart_mail_box_t;


/**
 * \brief This enum define the UART port to be used.
 */
typedef enum {UART_0,UART_1,UART_2,UART_3,UART_4,UART_5} uart_channel_t;

/**
 * \brief It defines some common transmission baud rates
 */
typedef enum {BD_4800 = 4800,BD_9600 = 9600,BD_5600 = 5600, BD_115200 = 115200} uart_baud_rate_t;


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function configure the callback on UART through a handler.
 	 \param[in]  UART channel to be used.
	 \param[in]  handler function to make the callback
 	 \return void
 */
void UART_callback_init(uart_channel_t uart_channel, void(*handler)(void));


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It configures the UART to be used
 	 \param[in]  uartChannel indicates which UART will be used.
 	 \param[in]  systemClk indicates the MCU frequency.
 	 \param[in]  baudRate sets the baud rate to transmit.
 	 \return void
 */
void UART_init(uart_channel_t uart_channel, uint32_t system_clk, uart_baud_rate_t baud_rate);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 enables the RX UART interrupt). This function should include the next sentence:
 	 while (!(UART0_S1 & UART_S1_RDRF_MASK)). It is to guaranty that the incoming data is complete
 	 when reception register is read. For more details see chapter 52 in the kinetis reference manual.
 	 \param[in]  uartChannel indicates the UART channel.
 	 \return void
 */
void UART_interrupt_enable(uart_channel_t uart_channel);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends one character through the serial port. This function should include the next sentence:
 	 while(!(UART0_S1 & UART_S1_TC_MASK)). It is to guaranty that before to try to transmit a byte, the previous
 	 one was transmitted. In other word, to avoid to transmit data while the UART is busy transmitting information.
 	 \param[in]  uartChannel indicates the UART channel.
 	 \param[in]  character to be transmitted.
 	 \return void
 */

void UART_put_char (uart_channel_t uart_channel, uint8_t character);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It sends a string character through the serial port.
 	 \param[in]  uartChannel indicates the UART channel.
 	 \param[in]  string pointer to the string to be transmitted.
 	 \return void
 */
void UART_put_string(uart_channel_t uart_channel, int8_t* string);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 check if the information has been received through the serial port
 	 \param[in]  uartChannel indicates the UART channel.
*/
void UART_RX_TX_IRQHandler(uart_channel_t uart_channel);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 check if the information has been received through the serial port
 	 \param[in]  uartChannel indicates the UART channel
 	 \return 	 interrupt flag
*/
uint8_t UART_get_interrupt_flag(uart_channel_t channel);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 check if the information has been received through the serial port
 	 \param[in]  uartChannel indicates the UART channel
 	 \return 	 void
*/
void UART_clear_interrupt_flag(uart_channel_t channel);



/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 check which information has been received through the serial port
 	 \param[in]  uartChannel indicates the UART channel
 	 \return 	 UART data
*/
uint8_t UART_get_mailbox(uart_channel_t channel);
#endif /* UART_H_ */
