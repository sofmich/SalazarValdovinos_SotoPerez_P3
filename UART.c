/**
	\file
	\brief
		This project shows how works the emulation of a VT 100 in teraterm.
	\author J. Sofia Salazar, Omar Soto
	\date	05/03/2019
	\todo
		Implement callbacks and handlers
 */

#include"UART.h"
#include "MK64F12.h"

/*This structure saves the interruption flag and data for each UART*/
uart_mail_box_t g_mail_box_uart[6] = {FALSE};
/*These are the handlers for UART interruption*/
static void (*UARTn_callback[6])(void) = {FALSE};


void UART_init(UART_config_t *config){

	/** Enable UART clock and pines*/
	UART_clockGating(config->uart_channel);
	/**Disable transmission and reception*/
	UART_Tx_Disable(config->uart_channel);
	UART_Rx_Disable(config->uart_channel);
	/** Set UART baud Rate to transmit*/
	UART_setBaudRate(config->uart_channel, config->system_clk,config->baud_rate);
	/** Enable transmission and reception*/
	UART_Tx_Enable(config->uart_channel);
	UART_Rx_Enable(config->uart_channel);
}

void UART_setBaudRate(uart_channel_t channel, uint32_t system_clk, uint32_t baud_rate)
{
	/**Baud Rate variables */
	uint16_t BR_UART;
	uint16_t BR_FA = 0;

	/**Calculate the UART baud rate*/
	BR_UART = (uint16_t)((system_clk) / (16 * (baud_rate)));
	BR_FA = (uint8_t)(2*((system_clk)/(baud_rate)) - (BR_UART * 32));

	switch(channel){
		case (UART_0):
			/**Load low and high part of the baud rate*/
			UART0->BDH = ((BR_UART >> 8) & UART_BDH_SBR_MASK);
			UART0->BDL = (BR_UART & UART_BDL_SBR_MASK);
			/**Increment resolution of 1/32*/
			UART0->C4 |= (BR_FA & UART_C4_BRFA_MASK);
			break;
		case(UART_1):
			/**Load low and high part of the baud rate*/
			UART1->BDH = ((BR_UART >> 8) & UART_BDH_SBR_MASK);
			UART1->BDL = (BR_UART & UART_BDL_SBR_MASK);
			/**Increment resolution of 1/32*/
			UART1->C4 |= (BR_FA & UART_C4_BRFA_MASK);
			break;
		case(UART_2):
			/**Load low and high part of the baud rate*/
			UART2->BDH = ((BR_UART >> 8) & UART_BDH_SBR_MASK);
			UART2->BDL = (BR_UART & UART_BDL_SBR_MASK);
			/**Increment resolution of 1/32*/
			UART2->C4 |= (BR_FA & UART_C4_BRFA_MASK);
			break;
		case(UART_3):
			/**Load low and high part of the baud rate*/
			UART3->BDH = ((BR_UART >> 8) & UART_BDH_SBR_MASK);
			UART3->BDL = (BR_UART & UART_BDL_SBR_MASK);
			/**Increment resolution of 1/32*/
			UART3->C4 |= (BR_FA & UART_C4_BRFA_MASK);
			break;
		case(UART_4):
			/**Load low and high part of the baud rate*/
			UART4->BDH = ((BR_UART >> 8) & UART_BDH_SBR_MASK);
			UART4->BDL = (BR_UART & UART_BDL_SBR_MASK);
			/**Increment resolution of 1/32*/
			UART4->C4 |= (BR_FA & UART_C4_BRFA_MASK);
			break;
		case(UART_5):
			/**Load low and high part of the baud rate*/
			UART5->BDH = ((BR_UART >> 8) & UART_BDH_SBR_MASK);
			UART5->BDL = (BR_UART & UART_BDL_SBR_MASK);
			/**Increment resolution of 1/32*/
			UART5->C4 |= (BR_FA & UART_C4_BRFA_MASK);
			break;
		}


}
void UART_interrupt_enable(uart_channel_t uart_channel){
	switch(uart_channel){
	case (UART_0):
		//while (!(UART0->S1 & UART_S1_RDRF_MASK));
		UART0->C2 |= UART_C2_RIE_MASK;
		break;
	case(UART_1):
		UART1->C2 |= UART_C2_RIE_MASK;
		break;
	case(UART_2):
		UART2->C2 |= UART_C2_RIE_MASK;
		break;
	case(UART_3):
		UART3->C2 |= UART_C2_RIE_MASK;
		break;
	case(UART_4):
		UART4->C2 |= UART_C2_RIE_MASK;
		break;
	case(UART_5):
		UART5->C2 |= UART_C2_RIE_MASK;
		//while (!(UART5->S1 & UART_S1_RDRF_MASK));
		break;
	}
}
void UART_put_char (uart_channel_t uart_channel, uint8_t character){
	 /**Once transmission is complete, then send another char*/
	switch(uart_channel){
	case (UART_0):
		while(!(UART0->S1 & UART_S1_TC_MASK));
		UART0->D = character;
		break;
	case(UART_1):
		while(!(UART1->S1 & UART_S1_TC_MASK));
		UART1->D = character;
		break;
	case(UART_2):
		while(!(UART2->S1 & UART_S1_TC_MASK));
		UART2->D = character;
		break;
	case(UART_3):
		while(!(UART3->S1 & UART_S1_TC_MASK));
		UART3->D = character;
		break;
	case(UART_4):
		while(!(UART4->S1 & UART_S1_TC_MASK));
		UART4->D = character;
		break;
	case(UART_5):
		while(!(UART5->S1 & UART_S1_TC_MASK));
		UART5->D = character;
		break;
	}
}

void UART_put_string(uart_channel_t uart_channel, int8_t* string){
	while('\0' != *string){
		UART_put_char(UART_0, *string);
		string++;
	}
}

void UART_clockGating(uart_channel_t channel)
{
	switch(channel)
	{
	case UART_0:
		SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
		/**Configures the pin control register of pin16 in PortB as UART0 RX*/
		PORTB->PCR[16] = PORT_PCR_MUX(3);
		/**Configures the pin control register of pin16 in PortB as UART0 TX*/
		PORTB->PCR[17] = PORT_PCR_MUX(3);
		break;
	case UART_1:
		SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
		break;
	case UART_2:
		SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
		break;
	case UART_3:
		SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;
		break;
	case UART_4:
		SIM->SCGC1 |= SIM_SCGC1_UART4_MASK;
		/**Configures the pin control register of pin16 in PortB as UART0 RX*/
		PORTC->PCR[14] = PORT_PCR_MUX(3);
		/**Configures the pin control register of pin16 in PortB as UART0 TX*/
		PORTC->PCR[15] = PORT_PCR_MUX(3);
		break;
	case UART_5:
		SIM->SCGC1 |= SIM_SCGC1_UART5_MASK;
		break;
	default:
		break;
	}
}

void UART_Tx_Enable(uart_channel_t channel)
{
	switch (channel) {
	case UART_0:
		UART0->C2 |= UART_C2_TE_MASK;
		break;
	case UART_1:
		UART1->C2 |= UART_C2_TE_MASK;
		break;
	case UART_2:
		UART2->C2 |= UART_C2_TE_MASK;
		break;
	case UART_3:
		UART3->C2 |= UART_C2_TE_MASK;
		break;
	case UART_4:
		UART4->C2 |= UART_C2_TE_MASK;
		break;
	case UART_5:
		UART5->C2 |= UART_C2_TE_MASK;
		break;
	default:
		break;
	}

}

void UART_Rx_Enable(uart_channel_t channel)
{
	switch (channel) {
	case UART_0:
		UART0->C2 |= UART_C2_RE_MASK;
		break;
	case UART_1:
		UART1->C2 |= UART_C2_RE_MASK;
		break;
	case UART_2:
		UART2->C2 |= UART_C2_RE_MASK;
		break;
	case UART_3:
		UART3->C2 |= UART_C2_RE_MASK;
		break;
	case UART_4:
		UART4->C2 |= UART_C2_RE_MASK;
		break;
	case UART_5:
		UART5->C2 |= UART_C2_RE_MASK;
		break;
	default:
		/* Does nothing*/
		break;
	}
}

void UART_Tx_Disable(uart_channel_t channel)
{
	switch (channel) {
	case UART_0:
		UART0->C2 &= ~UART_C2_TE_MASK;
		break;
	case UART_1:
		UART1->C2 &= ~UART_C2_TE_MASK;
		break;
	case UART_2:
		UART2->C2 &= ~UART_C2_TE_MASK;
		break;
	case UART_3:
		UART3->C2 &= ~UART_C2_TE_MASK;
		break;
	case UART_4:
		UART4->C2 &= ~UART_C2_TE_MASK;
		break;
	case UART_5:
		UART5->C2 &= ~UART_C2_TE_MASK;
		break;
	default:
		break;
	}
}

void UART_Rx_Disable(uart_channel_t channel)
{
	switch (channel) {
	case UART_0:
		UART0->C2 &= ~UART_C2_RE_MASK;
		break;
	case UART_1:
		UART1->C2 &= ~UART_C2_RE_MASK;
		break;
	case UART_2:
		UART2->C2 &= ~UART_C2_RE_MASK;
		break;
	case UART_3:
		UART3->C2 &= ~UART_C2_RE_MASK;
		break;
	case UART_4:
		UART4->C2 &= ~UART_C2_RE_MASK;
		break;
	case UART_5:
		UART5->C2 &= ~UART_C2_RE_MASK;
		break;
	default:
		break;
	}

}



void UART_callback_init(uart_channel_t uart_channel, void(*handler)(void))
{
	UARTn_callback[uart_channel] = handler;
}

void UART0_RX_TX_IRQHandler(void)
{
	if((UART0->S1 & UART_S1_RDRF_MASK)){
		g_mail_box_uart[UART_0].mailBox = UART0->D;
		g_mail_box_uart[UART_0].flag = TRUE;
		if(UARTn_callback[UART_0])
		{
			UARTn_callback[UART_0]();
		}
	}

}


void UART1_RX_TX_IRQHandler(void)
{
	if((UART1->S1 & UART_S1_RDRF_MASK)){
		g_mail_box_uart[UART_1].mailBox = UART1->D;
		g_mail_box_uart[UART_1].flag = TRUE;
		if(UARTn_callback[UART_1])
		{
			UARTn_callback[UART_1]();
		}
	}

}

void UART2_RX_TX_IRQHandler(void)
{
	if((UART2->S1 & UART_S1_RDRF_MASK)){
		g_mail_box_uart[UART_2].mailBox = UART2->D;
		g_mail_box_uart[UART_2].flag = TRUE;
		if(UARTn_callback[UART_2])
		{
			UARTn_callback[UART_2]();
		}
	}

}

void UART3_RX_TX_IRQHandler(void)
{
	if((UART3->S1 & UART_S1_RDRF_MASK)){
		g_mail_box_uart[UART_3].mailBox = UART0->D;
		g_mail_box_uart[UART_3].flag = TRUE;
		if(UARTn_callback[UART_3])
		{
			UARTn_callback[UART_3]();
		}
	}
}
void UART4_RX_TX_IRQHandler(void)
{
	if((UART4->S1 & UART_S1_RDRF_MASK)){
		g_mail_box_uart[UART_4].mailBox = UART0->D;
		g_mail_box_uart[UART_4].flag = TRUE;
		if(UARTn_callback[UART_4])
		{
			UARTn_callback[UART_4]();
		}
	}
}

uint8_t UART_get_interrupt_flag(uart_channel_t channel)
{

	return g_mail_box_uart[channel].flag;
}

void UART_clear_interrupt_flag(uart_channel_t channel)
{
	g_mail_box_uart[channel].flag = FALSE;
}


uint8_t UART_get_mailbox(uart_channel_t channel)
{
	return  g_mail_box_uart[channel].mailBox;

}

