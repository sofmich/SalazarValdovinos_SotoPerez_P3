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

/** Global mode s used to manipulate from infinite loop*/
uint8_t g_status = MAIN_MENU;
/** Global uart value to alternate between two terminals*/
uint8_t g_UART_in_use = FALSE;

/** Configuration for I2C baud rate*/
const i2c_baud_rate_t baud_rate = {2, 1 };

/** Pointer used to make callback point to direction zero and not execute any funciton*/
void (*clear_callback)(void) = FALSE;
/** Save data that will be transmitted through I2C*/
uint8_t g_set_rtc_time[HOUR_DIGITS] = {FALSE};
/** Save data that will be transmitted through I2C*/
uint8_t g_set_rtc_date[HOUR_DIGITS] = {FALSE};
/** Counter of times a digit is entered for setting hour*/
uint8_t g_counter_inputs_time  = FALSE;
/** Counter of times a digit is entered for setting date*/
uint8_t g_counter_inputs_date  = FALSE;
/** Variable to control the total inputs of set new hour/date*/
uint8_t g_full_set_flag_time = FALSE;
/** Variable to control the total inputs of set new hour/date*/
uint8_t g_full_set_flag_date = FALSE;
/** Flag to control wheter reading hour is on terminal o matrix*/
uint8_t g_flag_YES;
/** Array to control number of inputs*/
uint8_t g_INPUTS[INPUTS_YES] = {FALSE};
/** Terminal to use callback of time*/
uint8_t g_terminal_update_time = FALSE;
/** Terminal to use callback of date*/
uint8_t g_terminal_update_date = FALSE;
/** Input counter to YES action*/
uint8_t g_counter_inputs_yes = FALSE;



uint8_t g_UART_mode[MODES_ACCOUNT] = {NOT_A_TERMINAL};



ControlStatus Execute_Functionality[] = {
		FALSE,
		config_time,
		config_date,


};


void main_menu(UART_in_user_t terminal_in_use)
{

	/** Reinitialize all variables and data*/
	GPIO_callback_init(GPIO_D, clear_callback);
	display_main_menu(terminal_in_use);

}


void config_time(UART_in_user_t terminal_in_use, uint8_t data_from_user)
{

	if( g_counter_inputs_time < HOUR_DIGITS)
	{
		if(data_from_user >= ZERO_ASCII && data_from_user <= NINE_ASCII)
		{
			g_set_rtc_time[g_counter_inputs_time] = data_from_user;
			/**Sends to the PCA the received data in the mailbox*/
			UART_put_char (terminal_in_use, g_set_rtc_time[g_counter_inputs_time] );
			/**Convert ASCII to decimal*/
			g_set_rtc_time[g_counter_inputs_time]  = g_set_rtc_time[g_counter_inputs_time]  - ASCII_CONSTANT;
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
		/** Updates time acording if it is an update of hours or date*/
		UpdateTime(TIME, g_set_rtc_time);
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(terminal_in_use,"\033[13;10H");
		UART_put_string(terminal_in_use,"La hora ha sido configurada");
	}
	/** If doing this ESC key is pressed then it should go out */
	else if(ESC_ASCII == data_from_user && TRUE == g_full_set_flag_time)
	{
		*g_set_rtc_time = FALSE;
		g_counter_inputs_time  = FALSE;
		g_full_set_flag_time = FALSE;
		g_status = MAIN_MENU;
		for(uint8_t i = 0; i < MODES_ACCOUNT ; i++)
		{
			/** Check the terminal in use to make an exit and clean*/
			if(g_UART_mode[i] == terminal_in_use)
			{
				g_UART_mode[i] = NOT_A_TERMINAL;
			}
		}
		main_menu(terminal_in_use);
	}
}

void config_date(UART_in_user_t terminal_in_use, uint8_t data_from_user)
{
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
				UART_put_char (terminal_in_use, ':');
			}
		}
	}
	/**Once all set digits are completed the hour can be set*/
	else if(ENTER_ASCII == data_from_user)
	{
		/** Updates time acording if it is an update of hours or date*/
		UpdateTime(DATE, g_set_rtc_date);
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(terminal_in_use,"\033[13;10H");
		UART_put_string(terminal_in_use,"La hora ha sido configurada");
		g_full_set_flag_date = TRUE;
	}
	/** If doing this ESC key is pressed then it should go out */
	else if(ESC_ASCII == data_from_user && TRUE == g_full_set_flag_date)
	{
		*g_set_rtc_date = FALSE;
		g_counter_inputs_date  = FALSE;
		g_full_set_flag_date = FALSE;
		for(uint8_t i = 0; i < MODES_ACCOUNT ; i++)
		{
			/** Check the terminal in use to make an exit and clean*/
			if(g_UART_mode[i] == terminal_in_use)
			{
				g_UART_mode[i] = NOT_A_TERMINAL;
			}
		}
		main_menu(terminal_in_use);
	}
}

void init_system(void)
{
	/**Enables the clock of PortB in order to configures TX and RX of UART peripheral*/
	SIM->SCGC5 = SIM_SCGC5_PORTB_MASK;
	/**Configures UART 0 to transmit/receive at 11520 bauds with a 21 MHz of clock core*/
	UART_init (UART_0,  21000000, BD_115200);
	UART_init (UART_4,  21000000, BD_115200);
	/**Enables the UART 0 interrupt*/
	UART_interrupt_enable(UART_0);
	/**Enables the UART 0 interrupt*/
	UART_interrupt_enable(UART_4);

	/**Enables the UART 0 and PORTD interrupt in the NVIC*/
	NVIC_enable_interrupt_and_priotity(UART0_IRQ, PRIORITY_9);
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

	/** Configure RTC hardware to init 1Hz signal*/
	Init_MCP7940(I2C_0, baud_rate);
    /** Init Led Matrix*/
	MATRIX_init();

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
	UART_put_string(terminal_in_use,"\033[9;10H");
	UART_put_string(terminal_in_use, "Menu principal\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(terminal_in_use,"\033[10;10H");
	UART_put_string(terminal_in_use, "1) Leer Hora\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(terminal_in_use,"\033[11;10H");
	UART_put_string(terminal_in_use, "2) Configurar Hora\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(terminal_in_use,"\033[12;10H");
}

void display_default(UART_in_user_t terminal_in_use)
{

	/*VT100 command for clearing the screen*/
	UART_put_string(g_UART_in_use,"\033[2J");
	/** VT100 command for text in  bright, black and background in cyan*/
	UART_put_string(g_UART_in_use,"\033[1;30;47m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(g_UART_in_use,"\033[10;10H");
	switch(g_status)
	{
	case(SET_TIME):
		UART_put_string(terminal_in_use, "Introducir hora actual con el siguiente formato\r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(terminal_in_use,"\033[11;10H");
		UART_put_string(terminal_in_use, "HH:MM:SS      24HRS\r");
		g_UART_mode[SET_TIME] = terminal_in_use;
			break;
	case(SET_DATE):
		UART_put_string(terminal_in_use, "Introducir fecha con el siguiente formato\r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_put_string(terminal_in_use,"\033[11;10H");
		UART_put_string(terminal_in_use, "dd/mm/aa\r");
		g_UART_mode[SET_DATE] = terminal_in_use;
			break;
	case(READ_TIME):
	UART_put_string(terminal_in_use, "Elige en que te gustaria ver la hora");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(terminal_in_use,"\033[11;10H");
	UART_put_string(terminal_in_use, "1) Terminal");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(terminal_in_use,"\033[11;10H");
	UART_put_string(terminal_in_use, "2) Consola");
	g_UART_mode[READ_TIME]= terminal_in_use;
	break;
	case(READ_DATE):
	UART_put_string(terminal_in_use, "La fecha actual es");
	g_UART_mode[READ_DATE] = terminal_in_use;
	case(WRITE_MEMORY):
		UART_put_string(terminal_in_use, "Direccion de escritura\r");
	g_UART_mode[WRITE_MEMORY] = terminal_in_use;
			break;
	case(READ_MEMORY):
		UART_put_string(terminal_in_use, "Direccion de lectura\r");
		g_UART_mode[READ_MEMORY] = terminal_in_use;
	case(MATRIX_MESSAGE):
		UART_put_string(terminal_in_use, "Mensajes almacenados: \r");
	g_UART_mode[MATRIX_MESSAGE] = terminal_in_use;
	}
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(terminal_in_use,"\033[12;10H");
}

void UpdateDisplayTime(void)
{
	/** Structure to save all elements of time format*/
	time_format_t actual_time = GetGlobalTime();

	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[12;10H");
	/** Print chars of display time HH:MM:SS am*/
	uint8_t dots = ASCII_DOTS;
	UART_put_char(g_UART_in_use, actual_time.tens_hours + ASCII_CONST);
	UART_put_char(UART_0, actual_time.units_hours + ASCII_CONST);
	UART_put_char(UART_0, dots);
	UART_put_char(UART_0, actual_time.tens_minutes+ ASCII_CONST);
	UART_put_char(UART_0, actual_time.units_minutes + ASCII_CONST);
	UART_put_char(UART_0, dots);
	UART_put_char(UART_0, actual_time.tens_seconds + ASCII_CONST);
	UART_put_char(UART_0, actual_time.units_seconds + ASCII_CONST);
	switch(actual_time.am_pm)
	{
	case(AM):
		UART_put_string(UART_0,"   AM");
	break;
	case(PM):
		UART_put_string(UART_0,"   AM");
	break;
	}


}
void UpdateDisplayDate(void)
{
	date_format_t actual_date = GetGlobalDate();
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[12;10H");
	/** Print chars of display time HH:MM:SS am*/
	UART_put_char(UART_0, actual_date.tens_days + ASCII_CONST);
	UART_put_char(UART_0, actual_date.units_days + ASCII_CONST);
	UART_put_char(UART_0, '/');
	UART_put_char(UART_0, actual_date.tens_months + ASCII_CONST);
	UART_put_char(UART_0, actual_date.units_months  + ASCII_CONST);
	UART_put_char(UART_0, '/');
	UART_put_char(UART_0, actual_date.tens_years + ASCII_CONST);
	UART_put_char(UART_0, actual_date.units_years  + ASCII_CONST);
}



void read_time_menu(UART_in_user_t terminal_in_use, uint8_t data_from_user)
{
	/**If the last pressed key is ESC, go back to main menu*/
	/** Select between terminal o matrix*/
	if( FALSE == g_flag_YES)
	{
		if(ESC_ASCII == data_from_user)
		{
			for(uint8_t i = 0; i < MODES_ACCOUNT ; i++)
			{
				/** Check the terminal in use to make an exit and clean*/
				if(g_UART_mode[i] == terminal_in_use)
				{
					g_UART_mode[i] = NOT_A_TERMINAL;
				}
			}
			main_menu(terminal_in_use);
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
			UART_put_char(UART_0, data_from_user );
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
		g_INPUTS[g_counter_inputs_yes] = data_from_user;
		g_counter_inputs_yes++;
		/** Once yes or not is finished, check if ENTER was pressed*/
		if(INPUTS_YES >= g_counter_inputs_yes)
		{
			if(ENTER_ASCII == data_from_user)
			{
				if(g_INPUTS[0] == 'S' && g_INPUTS[1] == 'I')
				{
					/* Show time on MATRIX from here*/
					printf("Show time on matrix from here\n");
					GPIO_callback_init(GPIO_D, MATRIX_show_time);
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
				main_menu(terminal_in_use);
			}
		}
	}
}

void system_control(UART_in_user_t terminal_in_use ,uint8_t data_from_user)
{
	if(MAIN_MENU == g_status)
	{
		g_status = data_from_user;
		display_default(terminal_in_use);
	}
	/** If main menu is not active, find which state was active and which states corresponds to the terminal in use*/
	else{
		for(uint8_t i = 0; i < MODES_ACCOUNT ; i++)
		{
			/** If the terminal saved on uart and mode is active and matches the terminal in use*/
			if(g_UART_mode[i] == terminal_in_use)
			{
				/**Execute the function of the active terminal*/
				switch(i)
					{
					case(SET_TIME):
						config_time(terminal_in_use,data_from_user);
							break;
					case(SET_DATE):
						config_date(terminal_in_use,data_from_user);
							break;
					case(READ_TIME):
						read_time_menu(terminal_in_use, data_from_user);
							break;
					case(READ_DATE):
						/** Function to read date from here*/
						GPIO_callback_init(GPIO_D, UpdateDisplayDate);
						if(ESC_ASCII == data_from_user)
						{
							g_UART_mode[i] = NOT_A_TERMINAL;
							g_status = MAIN_MENU;
							main_menu(terminal_in_use);
						}
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
		}

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
	UART_put_string(terminal_in_use, "El modo ya esta en uso, selecciona otro\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(terminal_in_use,"\033[10;10H");
	main_menu(terminal_in_use);
}
