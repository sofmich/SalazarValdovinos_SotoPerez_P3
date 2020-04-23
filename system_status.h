/**
	\file	system_status.h
	\brief
		This API controls the flow of the program
	\author Sofia Salazar, Omar Soto
	\date	07/04/2020
	\todo
 */
#ifndef SYSTEM_STATUS_H_
#define SYSTEM_STATUS_H_

#include "MK64F12.h"

/** Constant that represent the ESC key ascii code */
#define ESC_ASCII 	27U
/** Constant that represent the DOTS key ascii code */
#define DOTS_ASCII 	58U
/** Constant that represent the ENTER key ascii code */
#define ENTER_ASCII	13U

/** Constant to convert from ascii to number */
#define ASCII_CONSTANT 48U
/** Constant that represent the zero key ascii code */
#define ZERO_ASCII 	48U
/** Constant that represent the nine key ascii code */
#define NINE_ASCII	57U
/** Amount of digits to configure time */
#define HOUR_DIGITS	6U
/** First dots position*/
#define FIRST_DOTS	2U
/** Second dots position*/
#define SECOND_DOTS	4U
/** Amount of inputs of YES answer*/
#define INPUTS_YES	2U
/** Length of addres destiny*/
#define ADDRESS_LENGTH	6U
/** Length of text to send destiny*/
#define DIGITS_LENGTH	2U
/** Data from main system terminal*/
typedef uint8_t UART_in_user_t;
/** Data type of pointer to control the program flow (state machine)*/
typedef void (*ControlStatus)(void);

/** Definitions of system status*/
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
/** Terminals to be used among the program*/
typedef enum{
	TERMINAL0,
	TERMINAL4
}terminal_t;
/** Error flag to indicate wheter there was a problematic connection*/
typedef struct
{
	uint8_t error_setTime;
	uint8_t error_setDate;
	uint8_t error_readTime;
	uint8_t error_readDate;
	uint8_t error_writeMem;
	uint8_t error_readMem;
}connection_error_t;
/** Structure to control RAM flow*/
typedef struct{
	uint8_t counter;
	uint8_t address_flag;
	uint8_t length_flag;
	uint8_t message_flag;
}memoryStagesFlag_t;

/** Structure of each terminal status and data*/
typedef struct
{
	uint8_t status;
	uint8_t data_from_user;
}UART_MODE_t;

/********************************************************************************************/
/*!
 	 \brief	Re starts the needed configurations
 	 \param[in]  void
 	 \return void
 */
void main_menu(UART_in_user_t terminal_in_use);
/********************************************************************************************/
/*!
 	 \brief	Set the configuration for time modifications
 	 \param[in]  void
 	 \return void
 */
void config_time(void);
/********************************************************************************************/
/*!
 	 \brief	Set the configuration for date modifications
 	 \param[in]  void
 	 \return void
 */
void config_date(void);
/********************************************************************************************/
/*!
 	 \brief	Shows the menu to display time format
 	 \param[in]  void
 	 \return void
 */
void read_time_menu(void);
/********************************************************************************************/
/*!
 	 \brief	Starts the mode chat
 	 \param[in]  void
 	 \return void
 */
void start_chat(void);
/********************************************************************************************/
/*!
 	 \brief	Make all initializations to make the program work
 	 \param[in]  void
 	 \return void
 */
void init_system(void);
/********************************************************************************************/
/*!
 	 \brief	Starts the mode chat
 	 \param[in]  void
 	 \return void
 */
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
 	 \brief	Display the main menu interfacee
 	 \param[in]  void
 	 \return void
 */
void UpdateDisplayDate(void);
/********************************************************************************************/
/*!
 	 \brief	Control the program flow
 	 \param[in]  terminal in use
 	 \param[in]  data detected from uart
 	 \return void
 */
void system_control(UART_in_user_t terminal_in_use, uint8_t data_from_user);
/********************************************************************************************/
/*!
 	 \brief	Set an error message when the mode is in use
 	 \param[in]  terminal in use
 	 \return void
 */
void mode_in_use(UART_in_user_t terminal_in_use);
/********************************************************************************************/
/*!
 	 \brief	Control the steps to write on RAM
 	 \param[in]
 	 \return void
 */
void write_memory(void);
/********************************************************************************************/
/*!
 	 \brief	Control the steps to read on RAM
 	 \param[in]
 	 \return void
 */
void read_memory(void);
/********************************************************************************************/
/*!
 	 \brief	Shows messages from RAM on matrix
 	 \param[in]
 	 \return void
 */
void show_message_matrix(void);
/********************************************************************************************/
/*!
 	 \brief	indicate when a mode is busy or select mode or iterate over actions
 	 \param[in]  t
 	 \return void
 */
void main_menu_action(void);
/********************************************************************************************/
/*!
 	 \brief	Determinates which terminal is operating over a mode
 	 \param[in]  actual status
 	 \return terminal in use for that status
 */
uint8_t knowTerminal(uint8_t status);
/********************************************************************************************/
/*!
 	 \brief	Determinates which terminal  data  is being used
 	 \param[in]  actual status
 	 \return terminal data in use for that status
 */
uint8_t knowTerminalData(uint8_t status);
/********************************************************************************************/
/*!
 	 \brief		Displays a message when the connection is not effective
 	 \param[in] mode that displays the error
 	 \return void
 */
void SYSTEM_I2C_fail(uint8_t mode);
/********************************************************************************************/
/*!
 	 \brief		Displays the read data from direction
 	 \param[in] pointer to array data
 	 \return false
 */
void SYSTEM_printReadRam(uint8_t *data);

#endif /* SYSTEM_STATUS_H_ */
