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


void main_menu(void);

void config_date_time(uint8_t data_from_user);
void read_hour(void);
void read_date(void);
void start_chat(uint8_t data_from_user);



uint8_t init_system(void);

void display_main_menu(void);
void display_default(void);

void system_control(uint8_t data_from_user);

#endif /* SYSTEM_STATUS_H_ */
