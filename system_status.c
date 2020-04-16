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
uint8_t g_set_hour_digit[HOUR_DIGITS] = {FALSE};
/** Counter of times a digit is entered*/
uint8_t g_counter_set_hour  = FALSE;

void main_menu(void)
{

	/** Reinitialize all variables and data*/
	GPIO_callback_init(GPIO_D, clear_callback);
	*g_set_hour_digit = FALSE;
	g_counter_set_hour  = FALSE;
	display_main_menu();

}
void read_hour(void)
{
	/*VT100 command for clearing the screen*/
	UART_put_string(UART_0,"\033[2J");
	/** VT100 command for text in  bright, black and background in cyan*/
	UART_put_string(UART_0,"\033[1;30;47m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[10;10H");
	UART_put_string(UART_0, "La hora actual es\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[11;10H");
	GPIO_callback_init(GPIO_D, UpdateDisplayTime);
}

void display_config_hour(void)
{

	/*VT100 command for clearing the screen*/
	UART_put_string(UART_0,"\033[2J");
	/** VT100 command for text in  bright, black and background in cyan*/
	UART_put_string(UART_0,"\033[1;30;47m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[10;10H");
	UART_put_string(UART_0, "Introducir hora actual con el siguiente formato\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[11;10H");
	UART_put_string(UART_0, "HH:MM:SS      24HRS\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[12;10H");
}
void config_hour(uint8_t data_from_user)
{

	if( g_counter_set_hour < HOUR_DIGITS)
	{
		if(data_from_user >= ZERO_ASCII && data_from_user <= NINE_ASCII)
		{
			g_set_hour_digit[g_counter_set_hour] = data_from_user;
			/**Sends to the PCA the received data in the mailbox*/
			UART_put_char (UART_0, g_set_hour_digit[g_counter_set_hour] );
			/**Convert ASCII to decimal*/
			g_set_hour_digit[g_counter_set_hour]  = g_set_hour_digit[g_counter_set_hour]  - ASCII_CONSTANT;
			g_counter_set_hour ++ ;
			if(FIRST_DOTS == g_counter_set_hour || SECOND_DOTS == g_counter_set_hour)
			{
				uint8_t dots = DOTS_ASCII;
				UART_put_char (UART_0, dots);
			}
		}
	}
	/**Once all hour digits are completed the hour can be set*/
	else if(ENTER_ASCII == data_from_user)
	{
		//UpdateTime(g_set_hour_digit);
		display_main_menu();
		g_status = MAIN_MENU;
	}
}


uint8_t get_status(void)
{
	return g_status;
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

	return MAIN_MENU;

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

void system_set_status(SYSTEM_MODE status)
{
	g_status = status;
}
