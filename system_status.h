/*
 * system_status.h
 *
 *  Created on: Apr 7, 2020
 *      Author: Sofia Slazar, Omar Soto
 */

#ifndef SYSTEM_STATUS_H_
#define SYSTEM_STATUS_H_

#include "MK64F12.h"

#define ESC_ASCII 	27U
#define DOTS_ASCII 	58U
#define ENTER_ASCII	13U

#define ASCII_CONSTANT 48U

#define ZERO_ASCII 	48U
#define NINE_ASCII	57U

#define HOUR_DIGITS	6U
#define FIRST_DOTS	2U
#define SECOND_DOTS	4U

#define INPUTS_YES	3U

#define MODES_ACCOUNT 	9U

#define NOT_A_TERMINAL	10U

typedef enum uint8_t{
	MAIN_MENU,
	SET_TIME,
	SET_DATE,
	READ_TIME,
	READ_DATE,
	WRITE_MEMORY,
	READ_MEMORY,
	CHAT,
	MATRIX_MESSAGE
} SYSTEM_MODE;



typedef uint8_t UART_in_user_t;
void main_menu(UART_in_user_t terminal_in_use);

void config_time(UART_in_user_t terminal_in_use,uint8_t data_from_user);
void config_date(UART_in_user_t terminal_in_use, uint8_t data_from_user);
void read_time_menu(UART_in_user_t terminal_in_use, uint8_t data_from_user);
void read_date(UART_in_user_t terminal_in_use);

void start_chat(uint8_t data_from_user);

typedef void * ControlStatus;

void init_system(void);

void display_main_menu(UART_in_user_t terminal_in_use);
void display_default(UART_in_user_t terminal_in_use);
/********************************************************************************************/
/*!
 	 \brief	Update the new time value on UART to let the digital clock be seen by the user
 	 \param[in]  void
 	 \return void
 */
void UpdateDisplayTime(void);
/********************************************************************************************/
/*!
 	 \brief	Display on UART the current date
 	 \param[in]  void
 	 \return void
 */

void UpdateDisplayDate(void);

void system_control(UART_in_user_t terminal_in_use, uint8_t data_from_user);
void mode_in_use(UART_in_user_t terminal_in_use);

#endif /* SYSTEM_STATUS_H_ */
