/**
	\file
	\brief
		This project shows how works the emulation of a VT 100 in teraterm.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	05/03/2019
	\todo
		To implement blocking code
 */

#include "MK64F12.h" /* include peripheral declarations */
#include "UART.h"/**UART device driver*/
#include "system_status.h"
#include "MCP7940.h"



#define DEBUG

#ifdef DEBUG
	#include "stdio.h"
#endif

#define SYSTEM_CLOCK 21000000


int main(void)
{
	/**Init all system*/
	uint8_t status = init_system();
	system_set_status(status);
	for(;;) {
		if(TRUE == UART_get_interrupt_flag(UART_0))
		{
			UART_clear_interrupt_flag(UART_0);
			/** Save the key the user has pressed*/
			uint8_t data_from_user = UART_get_mailbox(UART_0);
			/** Know which status is the actual one to make changes*/
			status = get_status();
			/** Always ESC key is pressed, return to main_menu*/
			if(ESC_ASCII == data_from_user)
			{
				system_set_status(MAIN_MENU);
				main_menu();
			}
			/** Once config option was activated, chars should go to update time*/
			else if(CONFIG == status)
			{
				config_hour(data_from_user);

			}
			if(MAIN_MENU == status)
			{
				/** When in main menu select wheter 1 or 2 was selected as an option*/
				switch(data_from_user)
				{
				case('1'):
				read_hour();
				system_set_status(READ);
				break;
				case('2'):
				display_config_hour();
				system_set_status(CONFIG);
				break;

				}
			}
		}
	}

	return 0;
}
