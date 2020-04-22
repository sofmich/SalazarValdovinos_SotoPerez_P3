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

#define INPUTS_YES	2U

#define MODES_ACCOUNT 	9U

#define NOT_A_TERMINAL	10U

#define ADDRESS_LENGTH	6U
#define DIGITS_LENGTH	2U

typedef enum{
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

typedef enum{
	TERMINAL0,
	TERMINAL4
}terminal_t;

typedef struct{
	uint8_t counter;
	uint8_t address_flag;
	uint8_t length_flag;
	uint8_t message_flag;
}memoryStagesFlag_t;


typedef struct
{
	uint8_t status;
	uint8_t data_from_user;

}UART_MODE_t;
typedef uint8_t UART_in_user_t;
void main_menu(UART_in_user_t terminal_in_use);

void config_time(void);
void config_date(void);
void read_time_menu(void);


void start_chat(void);

typedef void (*ControlStatus)(void);

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

void write_memory(void);
void read_memory(void);
void show_message_matrix(void);

void main_menu_action(void);


uint8_t knowTerminal(uint8_t status);
uint8_t knowTerminalData(uint8_t status);

void SYSTEM_I2C_fail(uint8_t mode);

#endif /* SYSTEM_STATUS_H_ */
