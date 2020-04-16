/*
 * system_status.h
 *
 *  Created on: Apr 7, 2020
 *      Author: nxf63348
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


typedef enum uint8_t{
	MAIN_MENU,
	READ,
	CONFIG
} SYSTEM_MODE;


void main_menu(void);
void read_hour(void);
void config_hour(uint8_t data_from_user);

void display_config_hour(void);

uint8_t get_status(void);
uint8_t init_system(void);

void display_main_menu(void);
void system_set_status(uint8_t status);

#endif /* SYSTEM_STATUS_H_ */
