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

		}
	}

	return 0;
}
