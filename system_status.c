/**
	\file
	\brief
		This API controls the flow of the program
	\author Sofia Salazar, Omar Soto
	\date	07/04/2020
	\todo
		To implement blocking code
 */

#include "UART.h"/**UART device driver*/
#include "bits.h"
#include "system_status.h"
#include "NVIC.h"
#include "MCP7940.h"
#include "GPIO.h"
#include "I2C.h"

/** Global mode s used to manipulate from infinite loop*/
SYSTEM_MODE g_status = MAIN_MENU;
/** Configuration for I2C baud rate*/
const i2c_baud_rate_t baud_rate = {2, 1 };

/** Pointer used to make callback point to direction zero and not execute any funciton*/
void (*clear_callback)(void) = FALSE;
/** Save data that will be transmitted through I2C*/
uint8_t g_set_rtc_digit[HOUR_DIGITS] = {FALSE};
/** Counter of times a digit is entered*/
uint8_t g_counter_inputs  = FALSE;
/** Variable to control the total inputs of set new hour/date*/
uint8_t g_full_set_flag = FALSE;
/** Flag to control wheter reading hour is on terminal o matrix*/
uint8_t g_flag_YES;
/** Array to control number of inputs*/
uint8_t g_INPUTS[INPUTS_YES] = {FALSE};


void main_menu(void)
{

	/** Reinitialize all variables and data*/
	GPIO_callback_init(GPIO_D, clear_callback);
	g_counter_inputs = FALSE;
	display_main_menu();

}


void config_date_time(uint8_t data_from_user)
{
	if( g_counter_inputs < HOUR_DIGITS)
	{
		if(data_from_user >= ZERO_ASCII && data_from_user <= NINE_ASCII)
		{
			g_set_rtc_digit[g_counter_inputs] = data_from_user;
			/**Sends to the PCA the received data in the mailbox*/
			UART_put_char (UART_0, g_set_rtc_digit[g_counter_inputs] );
			/**Convert ASCII to decimal*/
			g_set_rtc_digit[g_counter_inputs]  = g_set_rtc_digit[g_counter_inputs]  - ASCII_CONSTANT;
			g_counter_inputs ++ ;
			if(FIRST_DOTS == g_counter_inputs || SECOND_DOTS == g_counter_inputs)
			{
				if(SET_TIME == g_status)
				{	/*If the input corresponds to a time format dots divide the input*/
					UART_put_char (UART_0, ':');
				}
				else
				{  /*If the input corresponds to a date, a slash divides the input*/
					UART_put_char (UART_0, '/');
				}

			}
		}
	}
	/**Once all set digits are completed the hour can be set*/
	else if(ENTER_ASCII == data_from_user)
	{
		/** Updates time acording if it is an update of hours or date*/
		UpdateTime(g_status, g_set_rtc_digit);
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(UART_0,"\033[13;10H");
		if(SET_TIME == g_status)
		UART_put_string(UART_0,"La hora ha sido configurada");
		else
		UART_put_string(UART_0,"La fecha ha sido configurada");
		g_full_set_flag = TRUE;
	}
	/** If doing this ESC key is pressed then it should go out */
	else if(ESC_ASCII == data_from_user && TRUE == g_full_set_flag)
	{
		*g_set_rtc_digit = FALSE;
		g_counter_inputs  = FALSE;
		g_full_set_flag = FALSE;
		g_status = MAIN_MENU;
		main_menu();
	}
}

uint8_t init_system(void)
{
	/**Enables the clock of PortB in order to configures TX and RX of UART peripheral*/
	SIM->SCGC5 = SIM_SCGC5_PORTB_MASK;
	/**Configures UART 0 */
	/**Configures the pin control register of pin16 in PortB as UART RX*/
	PORTB->PCR[16] = PORT_PCR_MUX(3);
	/**Configures the pin control register of pin16 in PortB as UART TX*/
	PORTB->PCR[17] = PORT_PCR_MUX(3);
	/**Configures UART 0 to transmit/receive at 11520 bauds with a 21 MHz of clock core*/
	UART_init (UART_0,  21000000, BD_115200);
	/**Enables the UART 0 interrupt*/
	UART_interrupt_enable(UART_0);
	/**Enables the UART 0 and PORTD interrupt in the NVIC*/
	NVIC_enable_interrupt_and_priotity(UART0_IRQ, PRIORITY_9);
	NVIC_enable_interrupt_and_priotity(PORTD_IRQ, PRIORITY_10);

	display_main_menu();

	/** Set configurations for GPIOD port*/
	GPIO_clock_gating(GPIO_D);
	/** Configure port and pin as a pull and interruption*/
	gpio_pin_control_register_t MFG_config = INTR_FALLING_EDGE | GPIO_MUX1 | GPIO_PE | GPIO_PS;
	GPIO_pin_control_register(GPIO_D,bit_0, &MFG_config);
	/** Configure PIn as an input*/
	GPIO_data_direction_pin(GPIO_D, bit_0, GPIO_INPUT);
	/*Callback initialization to nothing**/
	GPIO_callback_init(GPIO_D, clear_callback);

	/** Configure RTC hardware to init 1Hz signal*/
	Init_MCP7940(I2C_0, baud_rate);

	/**Enables interrupts*/
	NVIC_global_enable_interrupts;

	g_status = MAIN_MENU;
	return g_status;

}

void display_main_menu(void)
{
	/*VT100 command for clearing the screen*/
	UART_put_string(UART_0,"\033[2J");
	/** VT100 command for text in  bright, black and background in cyan*/
	UART_put_string(UART_0,"\033[1;30;47m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[9;10H");
	UART_put_string(UART_0, "Menu principal\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[10;10H");
	UART_put_string(UART_0, "1) Leer Hora\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[11;10H");
	UART_put_string(UART_0, "2) Configurar Hora\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[12;10H");
}

void display_default(void)
{

	/*VT100 command for clearing the screen*/
	UART_put_string(UART_0,"\033[2J");
	/** VT100 command for text in  bright, black and background in cyan*/
	UART_put_string(UART_0,"\033[1;30;47m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[10;10H");
	switch(g_status)
	{
	case(SET_TIME):
		UART_put_string(UART_0, "Introducir hora actual con el siguiente formato\r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(UART_0,"\033[11;10H");
		UART_put_string(UART_0, "HH:MM:SS      24HRS\r");
			break;
	case(SET_DATE):
		UART_put_string(UART_0, "Introducir fecha con el siguiente formato\r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(UART_0,"\033[11;10H");
		UART_put_string(UART_0, "dd/mm/aa\r");
			break;
	case(READ_TIME):
	UART_put_string(UART_0, "Elige en que te gustaria ver la hora");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[11;10H");
	UART_put_string(UART_0, "1) Terminal");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[11;10H");
	UART_put_string(UART_0, "2) Consola");
	break;
	case(READ_DATE):
	UART_put_string(UART_0, "La fecha actual es");
	case(WRITE_MEMORY):
		UART_put_string(UART_0, "Direccion de escritura\r");
			break;
	case(READ_MEMORY):
		UART_put_string(UART_0, "Direccion de lectura\r");
	case(MATRIX_MESSAGE):
		UART_put_string(UART_0, "Mensajes almacenados: \r");
	}
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[12;10H");
}


void read_time_menu(uint8_t data_from_user)
{
	/**If the last pressed key is ESC, go back to main menu*/
	/** Select between terminal o matrix*/
	if( FALSE == g_flag_YES)
	{
		if(ESC_ASCII == data_from_user)
		{
			g_status = MAIN_MENU;
			main_menu();
		}
		/* See the time value on terminal*/
		if('1' == data_from_user)
		{
			GPIO_callback_init(GPIO_D, UpdateDisplayTime);
			UART_put_char (UART_0, data_from_user );
		}
			/* Show confirmation message*/
		else if('2' == data_from_user && FALSE == g_flag_YES)
		{
			UART_put_char (UART_0, data_from_user );
			UART_put_string(UART_0,"\033[13;10H");
			UART_put_string(UART_0, "Deseas mostrar la hora en la matriz de leds? [SI][NO]");
			/** VT100 command for positioning the cursor in x and y position*/
			UART_put_string(UART_0,"\033[14;10H");
			g_flag_YES = TRUE;
		}
	}
	else
	{
		UART_put_char (UART_0, data_from_user );
		g_INPUTS[g_counter_inputs] = data_from_user;
		g_counter_inputs++;
		/** Once yes or not is finished, check if ENTER was pressed*/
		if(INPUTS_YES == g_counter_inputs)
		{
			if(ENTER_ASCII == data_from_user)
			{
				if(g_INPUTS[0] == 'S' && g_INPUTS[1] == 'I')
				{
					/* Show time on MATRIX from here*/
					printf("Show time on matrix from here\n");
				}
				else
				{
					/** VT100 command for positioning the cursor in x and y position*/
					UART_put_string(UART_0,"\033[15;10H");
					UART_put_string(UART_0, "Presiona ESC para volver [ESC]");
				}

			}
			else if(ESC_ASCII == data_from_user )
			{
				g_status = MAIN_MENU;
				main_menu();
			}
		}
	}
}

void system_control(uint8_t data_from_user)
{
	/** Check if the data introduced goes to any of the menu options*/
	switch(g_status)
	{
	case(MAIN_MENU):
			g_status = data_from_user;
	display_default();
	break;
	case(SET_TIME):
		config_date_time(data_from_user);
			break;
	case(SET_DATE):
		config_date_time(data_from_user);
			break;
	case(READ_TIME):
		read_time_menu(data_from_user);
			break;
	case(READ_DATE):
			/** Function to read date from here*/
		UpdateDisplayDate();
			break;
	case(WRITE_MEMORY):
			/** Functionality to write memory */
			break;
	case(READ_MEMORY):
			/** Functionality to read memory*/
			break;
	case(CHAT):
			/** Functionality of chat*/
			break;
	default:
		break;
	}

}
