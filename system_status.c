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
#include "LED_MATRIX.h"
#include "RAM.h"
#include "delay.h"

/** Configuration structure for UARTs*/
UART_config_t config_UART0={
	UART_0,
	21000000,
	BD_115200
};
UART_config_t config_UART4={
	UART_4,
	21000000,
	BD_9600
};

/** Global mode s used to manipulate from infinite loop*/
uint8_t g_status = MAIN_MENU;
/** Pointer used to make callback point to direction zero and not execute any funciton*/
void (*clear_callback)(void) = FALSE;
/** Save data that will be transmitted through I2C*/
uint8_t g_set_rtc_time[HOUR_DIGITS] = {FALSE};
time_format_t g_set_rtc_time_ ;
/** Pointer to save all time values */
uint8_t *p_digits_time = &(g_set_rtc_time_.tens_hours);
/** Save data that will be transmitted through I2C*/
uint8_t g_set_rtc_date[HOUR_DIGITS] = {FALSE};
/** Counter of times a digit is entered for setting hour*/
uint8_t g_counter_inputs_time  = FALSE;
/** Counter of times a digit is entered for setting date*/
uint8_t g_counter_inputs_date  = FALSE;
/** Variable to control the total inputs of set new hour/date*/
uint8_t g_AM_PM = FALSE;
/** Variable to control the total inputs of set new hour/date*/
uint8_t g_full_set_flag_date = FALSE;
/** Flag to control whether reading hour is on terminal o matrix*/
uint8_t g_flag_YES;
/** Array to control number of inputs*/
uint8_t g_INPUTS[INPUTS_YES] = {FALSE};
/** Input counter to YES action*/
uint8_t g_counter_inputs_yes = FALSE;
/** Flags for each terminal and  mode*/
UART_MODE_t g_UART_mode[2];
uart_mail_box_t g_mail_box_uart_t[6] = {FALSE};
/** Flags for memory status*/
memoryStagesFlag_t g_writeRAM_flag = {FALSE, FALSE, FALSE, FALSE};
memoryStagesFlag_t g_readRAM_flag = {FALSE, FALSE, FALSE, FALSE};
RAM_Data_t g_writeRAM_Data ;
RAM_Data_t g_readRAM_Data ;
uint8_t g_TERMINAL;
uint8_t g_TIME_format = FALSE;
/** Structure to indicate all posible connection errors*/
connection_error_t flag_Error =
		{
				FALSE,
				FALSE,
				FALSE,
				FALSE,
				FALSE,
				FALSE,
				FALSE
		};
/** Pointer to iterate over error indicators*/
uint8_t *p_flagError = &(flag_Error.error_setTime);
/** An structure of pointers that prints the first messages on RAM*/
first_messages_t g_first_messages = {FALSE};
/** State machine of the program*/
ControlStatus System_Status[] =
{
		main_menu_action,
		config_time,
		config_date,
		read_time_menu,
		UpdateDisplayDate,
		write_memory,
		read_memory,
		start_chat
//		show_message_matrix
};
resources_t g_OcupiedResource = {FALSE};
uint8_t g_UART0_counter = FALSE;
uint8_t g_UART4_counter = FALSE;
uint8_t message_UART0[50] = {FALSE};
uint8_t g_chat_terminal = FALSE;
uint8_t message_UART4[50] = {FALSE};
void start_chat(void)
{
	uint8_t terminal_in_use = g_chat_terminal;  /* Determina si fue de la UART 0  o la UART4*/
	uint8_t data_from_user = UART_get_mailbox(terminal_in_use);
	//uint8_t counter = 0;
	if(terminal_in_use == UART_0)
		{
			if(g_UART0_counter < 50)
			{
				message_UART0[g_UART0_counter] =data_from_user ;
				g_UART0_counter++;
				UART_put_char (UART_0, data_from_user);
			}
			if(ENTER_ASCII == data_from_user && g_UART_mode[TERMINAL4].status == CHAT )
			{
				g_UART0_counter = FALSE;
				UART_put_string (UART_0, "TERMINAL0: ");
				UART_put_string (UART_0, message_UART0);
				UART_put_string(UART_0,"\033[1B");
				UART_put_string(UART_0,"\033[1D");

				UART_put_string (UART_4, "TERMINAL0: ");
				UART_put_string (UART_4, message_UART0);
				UART_put_string(UART_4,"\033[1B");
				UART_put_string(UART_4,"\033[1D");
			}
			else if(ESC_ASCII == data_from_user)
			{
				if( g_UART_mode[TERMINAL4].status == CHAT)
				{
					UART_put_string(UART_4,"\033[1B");
					UART_put_string(UART_4,"\033[1D");
					UART_put_string (UART_4, "TERMINAL1 se desconecto ");
				}
				g_UART0_counter = FALSE;

				main_menu(UART_0);
			}

		}
		else if(terminal_in_use == UART_4)
		{
			if(g_UART4_counter < 50)
			{
				message_UART4[g_UART4_counter] = data_from_user;
				g_UART4_counter++;
				UART_put_char (UART_4, data_from_user);
			}
			if(ENTER_ASCII == data_from_user && g_UART_mode[TERMINAL0].status == CHAT)
			{
				g_UART4_counter = FALSE;
				UART_put_string (UART_0, "TERMINAL1: ");
				UART_put_string (UART_0, message_UART4);
				UART_put_string(UART_0,"\033[1B");
				UART_put_string(UART_0,"\033[1D");

				UART_put_string (UART_4, "TERMINAL1: ");
				UART_put_string (UART_4, message_UART4);
				UART_put_string(UART_4,"\033[1B");
				UART_put_string(UART_4,"\033[1D");
				//UART_put_string(UART_4,"\033[1B");
				//UART_put_string(UART_4,"\033[1D");
			}
			else if(ESC_ASCII == data_from_user )
			{
				if( g_UART_mode[TERMINAL0].status == CHAT)
				{
					UART_put_string(UART_0,"\033[1B");
					UART_put_string(UART_0,"\033[1D");
					UART_put_string (UART_0, "TERMINAL4 se desconecto ");
				}
				g_UART4_counter = FALSE;
				main_menu(UART_4);
			}

		}

}

//void show_message_matrix(void)
//{
//	uint8_t terminal_in_use = knowTerminal(MATRIX_MESSAGE);
//	printf("TERMINAL %d", terminal_in_use);
//	g_first_messages = readFirstMessages();
//	/** VT100 command for positioning the cursor in x and y position*/
//	UART_put_string(terminal_in_use,"\033[12;10H");
//	UART_put_string(terminal_in_use,"1) ");
//	UART_put_string(terminal_in_use, g_first_messages.p_message1);
//	/** VT100 command for positioning the cursor in x and y position*/
//	UART_put_string(terminal_in_use,"\033[14;10H");
//	UART_put_string(terminal_in_use,"2) ");
//	UART_put_string(terminal_in_use, g_first_messages.p_message2);
//	/** VT100 command for positioning the cursor in x and y position*/
//	UART_put_string(terminal_in_use,"\033[16;10H");
//	UART_put_string(terminal_in_use,"3) ");
//	UART_put_string(terminal_in_use, g_first_messages.p_message1);
//}

void main_menu(UART_in_user_t terminal_in_use)
{
	if(UART_0 == terminal_in_use)
	{
		g_UART_mode[TERMINAL0].status = MAIN_MENU;
	}
	else if(UART_4 == terminal_in_use)
	{
		g_UART_mode[TERMINAL4].status = MAIN_MENU;
	}
	/** Reinitialize all variables and data*/
	GPIO_callback_init(GPIO_D, clear_callback);
	display_main_menu(terminal_in_use);
}

uint8_t knowTerminal(uint8_t status)
{
	uint8_t terminal = FALSE;
	if(g_UART_mode[TERMINAL0].status == status)
	{
		terminal = UART_0;
	}
	else if(g_UART_mode[TERMINAL4].status == status)
	{
		terminal = UART_4;
	}
	return terminal;
}
uint8_t knowTerminalData(uint8_t status)
{
	uint8_t terminal = FALSE;
	if(g_UART_mode[TERMINAL0].status == status)
	{
		terminal = g_UART_mode[TERMINAL0].data_from_user;
	}
	else if(g_UART_mode[TERMINAL4].status == status)
	{
		terminal = g_UART_mode[TERMINAL4].data_from_user;
	}
	return terminal;
}
void config_time(void)
{
	uint8_t terminal_in_use = knowTerminal(SET_TIME);
	uint8_t data_from_user = knowTerminalData(SET_TIME);
	if( g_counter_inputs_time < HOUR_DIGITS)
	{
		if(data_from_user >= ZERO_ASCII && data_from_user <= NINE_ASCII)
		{
			*(p_digits_time + g_counter_inputs_time) = data_from_user;
			/**Sends to the PCA the received data in the mailbox*/
			UART_put_char (terminal_in_use, data_from_user );
			/**Convert ASCII to decimal*/
			*(p_digits_time + g_counter_inputs_time) -= ASCII_CONSTANT;
			g_counter_inputs_time ++ ;
			if(FIRST_DOTS == g_counter_inputs_time || SECOND_DOTS == g_counter_inputs_time)
			{
					/*If the input corresponds to a time format dots divide the input*/
					UART_put_char (terminal_in_use, ':');
			}
		}
	}
	/**Once all set digits are completed the hour can be set*/
	else if(ENTER_ASCII == data_from_user)
	{
		/* First update time to 24 hors*/
		//setFormat(mode_24_hrs);
		/** Updates time acording if it is an update of hours or date*/
		UpdateTime(TIME, p_digits_time);
		/* Get back format to its used*/
		//setFormat(g_TIME_format);
		if(flag_Error.error_setTime != TRUE)
		{
			/** VT100 command for positioning the cursor in x and y position*/
			UART_put_string(terminal_in_use,"\033[13;10H");
			UART_put_string(terminal_in_use,"La hora ha sido configurada");
			UART_put_string(terminal_in_use,"\033[14;10H");
			UART_put_string(terminal_in_use,"[ESC] Salir");
		}

	}
	/** If doing this ESC key is pressed then it should go out */
	else if(ESC_ASCII == data_from_user)
	{
		flag_Error.error_setTime = FALSE;
		*g_set_rtc_time = FALSE;
		g_counter_inputs_time  = FALSE;
		g_status = MAIN_MENU;
		main_menu(terminal_in_use);
		g_OcupiedResource.RTC_ocupied = FALSE;
	}
}

void config_date(void)
{
	uint8_t terminal_in_use = knowTerminal(SET_DATE);
	uint8_t data_from_user = knowTerminalData(SET_DATE);
	/** If doing this ESC key is pressed then it should go out */
	if(ESC_ASCII == data_from_user)
	{
		flag_Error.error_setDate = FALSE;
		*g_set_rtc_date = FALSE;
		g_counter_inputs_date  = FALSE;
		g_full_set_flag_date = FALSE;
		g_OcupiedResource.RTC_ocupied = FALSE;
		main_menu(terminal_in_use);
	}
	if( g_counter_inputs_date < HOUR_DIGITS)
	{
		if(data_from_user >= ZERO_ASCII && data_from_user <= NINE_ASCII)
		{
			g_set_rtc_date[g_counter_inputs_date] = data_from_user;
			/**Sends to the PCA the received data in the mailbox*/
			UART_put_char (terminal_in_use, g_set_rtc_date[g_counter_inputs_date] );
			/**Convert ASCII to decimal*/
			g_set_rtc_date[g_counter_inputs_date]  = g_set_rtc_date[g_counter_inputs_date]  - ASCII_CONSTANT;
			g_counter_inputs_date ++ ;
			if(FIRST_DOTS == g_counter_inputs_date || SECOND_DOTS == g_counter_inputs_date)
			{
				/*If the input corresponds to a time format dots divide the input*/
				UART_put_char (terminal_in_use, '/');
			}
		}
	}
	/**Once all set digits are completed the hour can be set*/
	else if(ENTER_ASCII == data_from_user)
	{
		/** Updates time acording if it is an update of hours or date*/
		UpdateTime(DATE, g_set_rtc_date);
		if(flag_Error.error_setDate != TRUE)
		{
			/** VT100 command for positioning the cursor in x and y position*/
			UART_put_string(terminal_in_use,"\033[13;10H");
			UART_put_string(terminal_in_use,"La fecha ha sido configurada");
			/** VT100 command for positioning the cursor in x and y position*/
			UART_put_string(terminal_in_use,"\033[14;10H");
			UART_put_string(terminal_in_use,"[ESC] Para salir");
		}
		g_full_set_flag_date = TRUE;
	}

}

void init_system(void)
{
	/** Init all variables*/
	g_UART_mode[TERMINAL0].status = MAIN_MENU;
	g_UART_mode[TERMINAL0].data_from_user = FALSE;
	g_UART_mode[TERMINAL4].status = MAIN_MENU;
	g_UART_mode[TERMINAL4].data_from_user = FALSE;

	/**Enables the clock of PortB in order to configures TX and RX of UART peripheral*/
	SIM->SCGC5 = SIM_SCGC5_PORTB_MASK;
	/**Configures UART 0 to transmit/receive at 11520 bauds with a 21 MHz of clock core*/
	UART_init (&config_UART0);
	/**Enables the clock of PortC in order to configures TX and RX of UART peripheral*/
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	//GPIO_clock_gating(GPIO_E);
	/**Configures UART 0 to transmit/receive at 11520 bauds with a 21 MHz of clock core*/
	UART_init (&config_UART4);

	/**Enables the UART 0 interrupt*/
	UART_interrupt_enable(UART_0);
	/**Enables the UART 0 interrupt*/
	UART_interrupt_enable(UART_4);

	/**Enables the UART 0 and PORTD interrupt in the NVIC*/
	NVIC_enable_interrupt_and_priotity(UART0_IRQ, PRIORITY_9);
	NVIC_enable_interrupt_and_priotity(UART4_IRQ, PRIORITY_9);
	NVIC_enable_interrupt_and_priotity(PORTD_IRQ, PRIORITY_10);

	display_main_menu(UART_0);
	display_main_menu(UART_4);

	/** Set configurations for GPIOD port*/
	GPIO_clock_gating(GPIO_D);
	/** Configure port and pin as a pull and interruption*/
	gpio_pin_control_register_t MFG_config = INTR_FALLING_EDGE | GPIO_MUX1 | GPIO_PE | GPIO_PS;
	GPIO_pin_control_register(GPIO_D,bit_0, &MFG_config);
	/** Configure PIn as an input*/
	GPIO_data_direction_pin(GPIO_D, bit_0, GPIO_INPUT);
	/*Callback initialization to nothing**/
	GPIO_callback_init(GPIO_D, clear_callback);

	/** Init RAM to start working*/
	RAM_init();
	/** Configure RTC hardware to init 1Hz signal*/
	Init_MCP7940();
    /** Init Led Matrix*/
	MATRIX_init();
	/** Turn off all from matrix*/
	MATRIX_off();

	/**Enables interrupts*/
	NVIC_global_enable_interrupts;


}

void display_main_menu(UART_in_user_t terminal_in_use)
{
	/*VT100 command for clearing the screen*/
	UART_put_string(terminal_in_use,"\033[2J");
	/** VT100 command for text in  bright, black and background in cyan*/
	UART_put_string(terminal_in_use,"\033[1;30;47m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(terminal_in_use,"\033[7;10H");
	UART_put_string(terminal_in_use, "Menu principal\r");
	UART_put_string(terminal_in_use,"\033[8;10H");
	UART_put_string(terminal_in_use, "1)Establecer Hora\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(terminal_in_use,"\033[9;10H");
	UART_put_string(terminal_in_use, "2)Establecer Fecha\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(terminal_in_use,"\033[10;10H");
	UART_put_string(terminal_in_use, "3)Leer hora\r");
	UART_put_string(terminal_in_use,"\033[11;10H");
	UART_put_string(terminal_in_use, "4)Leer fecha\r");
	UART_put_string(terminal_in_use,"\033[12;10H");
	UART_put_string(terminal_in_use, "5)Escribir en memoria\r");
	UART_put_string(terminal_in_use,"\033[13;10H");
	UART_put_string(terminal_in_use, "6)Leer de memoria\r");
	UART_put_string(terminal_in_use,"\033[14;10H");
	UART_put_string(terminal_in_use, "7)Chat\r");
	UART_put_string(terminal_in_use,"\033[15;10H");
//	UART_put_string(terminal_in_use, "8)Mostrar mensaje matriz de leds\r");
//	UART_put_string(terminal_in_use,"\033[16;10H");
}

void display_default(UART_in_user_t terminal_in_use)
{
	uint8_t TERMINAL;
	switch(terminal_in_use)/** To save UART status*/
	{
	case(UART_0):
			TERMINAL = TERMINAL0;
			break;
	case(UART_4):
			TERMINAL = TERMINAL4;
	break;
	}
	/*VT100 command for clearing the screen*/
	UART_put_string(terminal_in_use,"\033[2J");
	/** VT100 command for text in  bright, black and background in cyan*/
	UART_put_string(terminal_in_use,"\033[1;30;47m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(terminal_in_use,"\033[9;10H");
	switch(g_UART_mode[TERMINAL].status)
	{
	case(SET_TIME):
	{
		UART_put_string(terminal_in_use, "Introducir hora actual con el siguiente formato\r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(terminal_in_use,"\033[11;10H");
		UART_put_string(terminal_in_use, "HH:MM:SS      24HRS\r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(terminal_in_use,"\033[12;10H");
		g_OcupiedResource.RTC_ocupied = TRUE;
	}
			break;
	case(SET_DATE):
	{
		UART_put_string(terminal_in_use, "Introducir fecha con el siguiente formato\r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(terminal_in_use,"\033[11;10H");
		UART_put_string(terminal_in_use, "dd/mm/aa\r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(terminal_in_use,"\033[12;10H");
		g_OcupiedResource.RTC_ocupied = TRUE;
	}
	break;
	case(READ_TIME):
	{
		UART_put_string(terminal_in_use, "Elige en que te gustaria ver la hora");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(terminal_in_use,"\033[11;10H");
		UART_put_string(terminal_in_use,"1) Ver hora en consola 	2)Ver hora en matriz      ");
		g_OcupiedResource.RTC_ocupied = TRUE;
	}
	break;
	case(READ_DATE):
	{
		UART_put_string(terminal_in_use, "La fecha actual es");
		UpdateDisplayDate();
		g_OcupiedResource.RTC_ocupied = TRUE;
	}
	break;
	case(WRITE_MEMORY):
	{
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(terminal_in_use,"\033[9;10H");
		UART_put_string(terminal_in_use, "Direccion de escritura en formato 0x0000 \r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(terminal_in_use,"\033[10;10H");
		g_OcupiedResource.RAM_ocupied = TRUE;
	}
			break;
	case(READ_MEMORY):
	{
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(terminal_in_use,"\033[11;10H");
		UART_put_string(terminal_in_use, "Direccion de lectura\r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(terminal_in_use,"\033[12;10H");
		g_OcupiedResource.RTC_ocupied = TRUE;
	}
		break;
	case(CHAT):
	    {
	        /** VT100 command for positioning the cursor in x and y position*/
	        UART_put_string(terminal_in_use,"\033[9;10H");
	        UART_put_string(terminal_in_use, "Chateando con otra terminal\r");
	        /** VT100 command for positioning the cursor in x and y position*/
	        UART_put_string(terminal_in_use,"\033[10;10H");
	        g_OcupiedResource.RTC_ocupied = TRUE;
	    }
	        break;
//	case(MATRIX_MESSAGE):
//	{
//		/** VT100 command for positioning the cursor in x and y position*/
//		UART_put_string(terminal_in_use,"\033[9;10H");
//		UART_put_string(terminal_in_use, "Mensajes almacenados: \r");
//		/** VT100 command for positioning the cursor in x and y position*/
//		UART_put_string(terminal_in_use,"\033[10;10H");
//		System_Status[MATRIX_MESSAGE]();
//	}
//	break;
	default:
		break;
	}

}

void UpdateDisplayTime(void)
{
	/** Structure to save all elements of time format*/
	time_format_t actual_time = GetGlobalTime();
	uint8_t UART_in_use = knowTerminal(READ_TIME);

	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_in_use,"\033[14;10H");
	/** Print chars of display time HH:MM:SS am*/
	uint8_t dots = ASCII_DOTS;
	UART_put_char(UART_in_use, actual_time.tens_hours + ASCII_CONST);
	UART_put_char(UART_in_use, actual_time.units_hours + ASCII_CONST);
	UART_put_char(UART_in_use, dots);
	UART_put_char(UART_in_use, actual_time.tens_minutes+ ASCII_CONST);
	UART_put_char(UART_in_use, actual_time.units_minutes + ASCII_CONST);
	UART_put_char(UART_in_use, dots);
	UART_put_char(UART_in_use, actual_time.tens_seconds + ASCII_CONST);
	UART_put_char(UART_in_use, actual_time.units_seconds + ASCII_CONST);
	UART_put_string(UART_in_use, "       HRS");

}
void UpdateDisplayDate(void)
{
	/** Structure to save all elements of time format*/
	time_format_t actual_time = GetGlobalTime();
	uint8_t UART_in_use = knowTerminal(READ_DATE);
	uint8_t data_from_user = knowTerminalData(READ_DATE);

	date_format_t actual_date = GetGlobalDate();
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_in_use,"\033[11;10H");
	/** Print chars of display time HH:MM:SS am*/
	UART_put_char(UART_in_use, actual_date.tens_days + ASCII_CONST);
	UART_put_char(UART_in_use, actual_date.units_days + ASCII_CONST);
	UART_put_char(UART_in_use, '/');
	UART_put_char(UART_in_use, actual_date.tens_months + ASCII_CONST);
	UART_put_char(UART_in_use, actual_date.units_months  + ASCII_CONST);
	UART_put_char(UART_in_use, '/');
	UART_put_char(UART_in_use, actual_date.tens_years + ASCII_CONST);
	UART_put_char(UART_in_use, actual_date.units_years  + ASCII_CONST);
	if(ESC_ASCII == data_from_user)
	{
		main_menu(UART_in_use);
		g_OcupiedResource.RTC_ocupied = FALSE;
	}
}



void read_time_menu(void)
{
	uint8_t terminal_in_use = knowTerminal(READ_TIME);
	uint8_t data_from_user = knowTerminalData(READ_TIME);
	/**If the last pressed key is ESC, go back to main menu*/
	/** Select between terminal o matrix*/
	if(ESC_ASCII == data_from_user)
	{
		main_menu(terminal_in_use);
		g_AM_PM = FALSE;
		g_flag_YES = FALSE;
		g_OcupiedResource.RTC_ocupied = FALSE;
		MATRIX_off();
		g_counter_inputs_yes = FALSE;
	}
	else if( FALSE == g_flag_YES)
	{
		/* See the time value on terminal*/
		if('1' == data_from_user)
		{
			GPIO_callback_init(GPIO_D, UpdateDisplayTime);
			UART_put_char (UART_0, data_from_user );
		}
			/* Show confirmation message*/
		else if('2' == data_from_user)
		{
			UART_put_char(UART_0, data_from_user );
			UART_put_string(UART_0,"\033[14;10H");
			UART_put_string(UART_0, "Deseas mostrar la hora en la matriz de leds? [SI][NO]");
			/** VT100 command for positioning the cursor in x and y position*/
			UART_put_string(UART_0,"\033[15;10H");
			g_flag_YES = TRUE;
		}
	}
	else
	{
		if(g_counter_inputs_yes < INPUTS_YES)
		{
			UART_put_char (UART_0, data_from_user );
			g_INPUTS[g_counter_inputs_yes] = data_from_user;
			g_counter_inputs_yes++;
		}

		/** Once yes or not is finished, check if ENTER was pressed and SI was selected*/
		else if(ENTER_ASCII == data_from_user)
		{
			if(g_INPUTS[0] == 'S' && g_INPUTS[1] == 'I')
			{
				/** VT100 command for positioning the cursor in x and y position*/
				UART_put_string(UART_0,"\033[16;10H");
				UART_put_string(UART_0, "Viendo hora actual en matriz de leds");
				GPIO_callback_init(GPIO_D, MATRIX_show_time);
			}
			else
			{
				/** VT100 command for positioning the cursor in x and y position*/
				UART_put_string(UART_0,"\033[16;10H");
				UART_put_string(UART_0, "Presiona ESC para volver [ESC]");
			}
		}
		else if(ESC_ASCII == data_from_user )
		{
			MATRIX_off();
			g_counter_inputs_yes = FALSE;
			main_menu(terminal_in_use);
		}
	}
}

void write_memory(void)
{
	uint8_t terminal_in_use = knowTerminal(WRITE_MEMORY);
	uint8_t data_from_user = knowTerminalData(WRITE_MEMORY);

		/**If the last pressed key is ESC, go back to main menu*/
	/** Select between terminal o matrix*/
	if(ESC_ASCII == data_from_user)
	{
		g_writeRAM_flag.address_flag = FALSE;
		g_writeRAM_flag.length_flag = FALSE;
		g_writeRAM_flag.message_flag = FALSE;
		g_writeRAM_flag.counter = FALSE;
		flag_Error.error_writeMem =FALSE;
		g_OcupiedResource.RAM_ocupied = FALSE;
		main_menu(terminal_in_use);
	}
	else if( FALSE == g_writeRAM_flag.address_flag)
	{
		if(g_writeRAM_flag.counter < ADDRESS_LENGTH)
		{
			/** VT100 command for positioning the cursor in x and y position*/
			UART_put_char(terminal_in_use, data_from_user);
			/** Start saving the address*/
			g_writeRAM_Data.address[g_writeRAM_flag.counter] = data_from_user;
			g_writeRAM_flag.counter++;
		}
		else
		{
			/** VT100 command for positioning the cursor in x and y position*/
			UART_put_string(terminal_in_use,"\033[11;10H");
			UART_put_string(terminal_in_use, "Longitud del mensaje: (Max 99) Ej. 06, 12, 20\r");
			UART_put_string(terminal_in_use,"\033[12;10H");
			g_writeRAM_flag.address_flag = TRUE;
			g_writeRAM_flag.counter = FALSE;
		}
	}
	else if(FALSE == g_writeRAM_flag.length_flag)
	{
		if(g_writeRAM_flag.counter < DIGITS_LENGTH)
		{
			/** VT100 command for positioning the cursor in x and y position*/
			UART_put_char(terminal_in_use, data_from_user);
			/** Start saving the address*/
			g_writeRAM_Data.length[g_writeRAM_flag.counter] = data_from_user - ASCII_CONST;
			g_writeRAM_flag.counter++;
		}
		else
		{
			/** VT100 command for positioning the cursor in x and y position*/
			UART_put_string(terminal_in_use,"\033[15;10H");
			UART_put_string(terminal_in_use, "Mensaje a escribir: \r");
			UART_put_string(terminal_in_use,"\033[16;10H");
			g_writeRAM_flag.length_flag = TRUE;
			g_writeRAM_flag.counter = FALSE;
		}
	}
	else if(FALSE == g_writeRAM_flag.message_flag)
	{
		uint8_t message_length = g_writeRAM_Data.length[0] * 10 + g_writeRAM_Data.length[1];

		if(g_writeRAM_flag.counter < message_length )
		{
			/** VT100 command for positioning the cursor in x and y position*/
			UART_put_char(terminal_in_use, data_from_user);
			/** Start saving the data to write*/
			(g_writeRAM_Data.message[g_writeRAM_flag.counter] ) = data_from_user;
			g_writeRAM_flag.counter++;
		}
		else
		{
			g_writeRAM_flag.message_flag = TRUE;
			g_writeRAM_flag.counter = FALSE;

		}
	}
	else
	{
		/** Send the message*/
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(terminal_in_use,"\033[17;10H");
		UART_put_string(terminal_in_use, "Presiona ENTER para enviar\r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(terminal_in_use,"\033[18;10H");
		if(ENTER_ASCII == data_from_user)
		{
			RAM_prepareTo(RAM_write_, g_writeRAM_Data);
			if(flag_Error.error_writeMem != TRUE)
			{
				UART_put_string(terminal_in_use, "Tu mensaje ha sido enviado\r");
			}

		}
	}
}
void read_memory(void)
{
	uint8_t terminal_in_use = knowTerminal(READ_MEMORY);
	uint8_t data_from_user = knowTerminalData(READ_MEMORY);

		/**If the last pressed key is ESC, go back to main menu*/
	/** Select between terminal o matrix*/
	if(ESC_ASCII == data_from_user)
	{
		g_OcupiedResource.RAM_ocupied = FALSE;
		g_readRAM_flag.address_flag = FALSE;
		g_readRAM_flag.length_flag = FALSE;
		g_readRAM_flag.message_flag = FALSE;
		g_readRAM_flag.counter = FALSE;
		flag_Error.error_readMem = FALSE;
		main_menu(terminal_in_use);
	}
	else if( FALSE == g_readRAM_flag.address_flag)
	{
		if(g_readRAM_flag.counter < ADDRESS_LENGTH)
		{
			/** VT100 command for positioning the cursor in x and y position*/
			UART_put_char(terminal_in_use, data_from_user);
			/** Start saving the address*/
			g_readRAM_Data.address[g_readRAM_flag.counter] = data_from_user;
			g_readRAM_flag.counter++;
		}
		else
		{
			/** VT100 command for positioning the cursor in x and y position*/
			UART_put_string(terminal_in_use,"\033[13;10H");
			UART_put_string(terminal_in_use, "Longitud del mensaje: (Max 99) Ej. 06, 12, 20\r");
			UART_put_string(terminal_in_use,"\033[14;10H");
			g_readRAM_flag.address_flag = TRUE;
			g_readRAM_flag.counter = FALSE;
		}
	}
	else if(FALSE == g_readRAM_flag.length_flag)
	{
		if(g_readRAM_flag.counter < DIGITS_LENGTH)
		{
			/** VT100 command for positioning the cursor in x and y position*/
			UART_put_char(terminal_in_use, data_from_user);
			/** Start saving the address*/
			g_readRAM_Data.length[g_readRAM_flag.counter] = data_from_user - ASCII_CONST;
			g_readRAM_flag.counter++;
			if(g_readRAM_flag.counter ==  DIGITS_LENGTH)
			{
				/** VT100 command for positioning the cursor in x and y position*/
				UART_put_string(terminal_in_use,"\033[15;10H");
				UART_put_string(terminal_in_use, "Presiona [ENTER] para mostrar mensaje\r");
				UART_put_string(terminal_in_use,"\033[16;10H");
				g_readRAM_flag.length_flag = TRUE;
				g_readRAM_flag.counter = FALSE;
			}
		}

	}
	else if(ENTER_ASCII == data_from_user)
	{
		RAM_prepareTo(RAM_read, g_readRAM_Data);
	}
}

void mode_in_use(UART_in_user_t terminal_in_use)
{
	/*VT100 command for clearing the screen*/
	UART_put_string(terminal_in_use,"\033[2J");
	/** VT100 command for text in  bright, black and background in cyan*/
	UART_put_string(terminal_in_use,"\033[1;30;47m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(terminal_in_use,"\033[9;10H");
	UART_put_string(terminal_in_use, "El recurso ya esta en uso, selecciona otro\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(terminal_in_use,"\033[10;10H");
	delay(2000000);
	main_menu(terminal_in_use);
}

void main_menu_action(void)
{
	uint8_t indicate_busy = FALSE;
	uint8_t terminal_in_use = g_status;
	uint8_t data_from_user = g_UART_mode[g_TERMINAL].data_from_user;
	/** If a terminal is in use in a state whom matches the one from user, display error*/
	if(g_UART_mode[TERMINAL0].status == CHAT ||g_UART_mode[TERMINAL4].status == CHAT)
	{
		g_UART_mode[g_TERMINAL].status = data_from_user - ASCII_CONSTANT;
		display_default(terminal_in_use);
	}
	else if(g_UART_mode[TERMINAL0].status == data_from_user - ASCII_CONSTANT
			|| g_UART_mode[TERMINAL4].status == data_from_user - ASCII_CONSTANT)
	{
		indicate_busy = TRUE;
	}
	/** If a terminal in use matched the status from user, then it can not be used*/
	if(indicate_busy)
	{
		mode_in_use(terminal_in_use);
	}
	else /** Mode can be used for whether terminal*/
	{
		g_UART_mode[g_TERMINAL].status = data_from_user - ASCII_CONSTANT;
		display_default(terminal_in_use);
	}

}

void system_control(UART_in_user_t terminal_in_use ,uint8_t data_from_user)
{
	uint8_t actualStatusAction = FALSE;
	uint8_t TERMINAL;
	g_status = terminal_in_use;
	/** Check terminal in use to set its status flag*/
	switch(terminal_in_use)
	{
	case(UART_0):
	{
		actualStatusAction = g_UART_mode[TERMINAL0].status;
		g_UART_mode[TERMINAL0].data_from_user = data_from_user;
		g_TERMINAL = TERMINAL0;
		TERMINAL = TERMINAL0;
		g_chat_terminal= UART_0;
	}
	break;
		case(UART_4):
	{
		actualStatusAction = g_UART_mode[TERMINAL4].status;
		g_UART_mode[TERMINAL4].data_from_user = data_from_user;
		TERMINAL = TERMINAL4;
		g_TERMINAL = TERMINAL4;
		g_chat_terminal= UART_4;
	}
	break;
	}
	/** Alternate actions between terminals status*/
	System_Status[actualStatusAction]();
}

void SYSTEM_I2C_fail(uint8_t mode)
{

	if(READ_TIME == mode)
	{
		GPIO_callback_init(GPIO_D, clear_callback);
	}
	uint8_t terminal_in_use = knowTerminal(mode);
	/** Set a flag mode error to TRUE when this happens*/
	*(p_flagError + (mode-1)) = TRUE;
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(terminal_in_use,"\033[20;10H");
	UART_put_string(terminal_in_use, "Error de conexion\r");
	UART_put_string(terminal_in_use,"\033[21;10H");
	UART_put_string(terminal_in_use, "Asegura tu conexion [ESC]\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(terminal_in_use,"\033[9;10H");
}

void SYSTEM_printReadRam(uint8_t *data)
{
	if(FALSE == flag_Error.error_readMem )
	{
		uint8_t terminal_in_use = knowTerminal(READ_MEMORY);
		uint8_t i = FALSE;
		UART_put_string(terminal_in_use, data );
		UART_put_string(terminal_in_use, "    [ESC]" );

	}


}
