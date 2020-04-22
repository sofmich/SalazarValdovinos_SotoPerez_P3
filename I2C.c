/**
	\file
	\brief
		This is the API for the I2C device driver for Kinetis FRDMK64.
		i.e., this is the application programming interface (API) for the I2C peripheral.
	\author Sofía Salazar, Omar Soto
	\date	06/04/2020
	\todo
	    Check on all functions
 */

#include "I2C.h"
#include "mK64F12.h"
#include "GPIO.h"


void I2C_init(i2c_config_struct_t *config)
{
	I2C_clock_gating(config->channel);
	I2C_baud_rate(config-> channel, config->multiplier, config->clock_rate);
	I2C_enable(config->channel);
}

void I2C_baud_rate(i2c_channel_t channel, uint8_t multiplier, uint8_t clock_rate)
{
	switch(channel)
	{
	case(I2C_0):
		/** Assign the Mult value and the ICR value*/
		I2C0->F |= 	(multiplier);
		I2C0->F |=  (clock_rate);
	break;
	case(I2C_1):
		/** Assign the Mult value and the ICR value*/
		I2C1->F |= 	I2C_F_MULT(multiplier);
		I2C1->F |=  I2C_F_ICR(clock_rate);
	break;
	case(I2C_2):
		/** Assign the Mult value and the ICR value*/
		I2C2->F |= 	I2C_F_MULT(multiplier);
		I2C2->F |=  I2C_F_ICR(clock_rate);
	break;

	}

}
void I2C_enable(i2c_channel_t channel)
{
	switch(channel)
	{
	case(I2C_0):
		/** Enable the I2C0 module*/
		I2C0->C1 |= I2C_C1_IICEN_MASK;
		/** An Acknowledge Not-Acknowledge can be sent **/
		I2C0->SMB |= I2C_SMB_FACK_MASK;
	break;
	case(I2C_1):
		/** An Acknowledge Not-Acknowledge can be sent **/
		I2C1->SMB |= I2C_SMB_FACK_MASK;
		/** Enable the I2C0 module*/
		I2C1->C1 |= I2C_C1_IICEN_MASK;
	break;
	case(I2C_2):
		/** An Acknowledge Not-Acknowledge can be sent **/
		I2C2->SMB |= I2C_SMB_FACK_MASK;
		/** Enable the I2C0 module*/
		I2C2->C1 |= I2C_C1_IICEN_MASK;
	break;
	default:
		break;
	}

}

void I2C_clock_gating(i2c_channel_t channel)
{
	switch(channel)
	{
		case I2C_0:
			SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
			PORTB->PCR[2] = PORT_PCR_MUX(2);
			/**Configures the pin control register of pin16 in PortB as UART0 TX*/
			PORTB->PCR[3] = PORT_PCR_MUX(2);
			break;
		case I2C_1:
			SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;
			break;
		case I2C_2:
			/** Enable the clock gating for I2C0 */
			SIM->SCGC1 |= SIM_SCGC1_I2C2_MASK;
			break;
		default:
			break;
	}
}

void I2C_mst_or_slv_mode(i2c_channel_t channel, master_or_slave_t mst_or_slv)
{
	switch(channel){
	case(I2C_0):
		if(MASTER == mst_or_slv)
		{
			/*Set as master*/
			I2C0->C1 |= I2C_C1_MST_MASK;
		}
		else
		{
			/*Set as slave*/
			I2C0->C1 &= ~I2C_C1_MST_MASK; //Set as master
		}
	break;
	case(I2C_1):
		if(MASTER == mst_or_slv)
		{
			/*Set as master*/
			I2C1->C1 |= I2C_C1_MST_MASK;
		}
		else
		{
			/*Set as slave*/
			I2C1->C1 &= ~I2C_C1_MST_MASK; //Set as master
		}
	break;
	case(I2C_2):
		if(MASTER == mst_or_slv)
		{
			/*Set as master*/
			I2C2->C1 |= I2C_C1_MST_MASK;
		}
		else
		{
			/*Set as slave*/
			I2C2->C1 &= ~I2C_C1_MST_MASK; //Set as master
		}
	break;
	}
}

void I2C_tx_rx_mode(i2c_channel_t channel, uint8_t tx_or_rx)
{
	switch(channel){
	case(I2C_0):
		if(TRANSMITTER == tx_or_rx)
		{
			/*Set as transmitter*/
			I2C0->C1 |= I2C_C1_TX_MASK;
		}
		else
		{
			/*Set as receiver*/
			I2C0->C1 &= ~I2C_C1_TX_MASK;
		}
	break;
	case(I2C_1):
		if(TRANSMITTER == tx_or_rx)
		{
			/*Set as transmitter*/
			I2C1->C1 |= I2C_C1_TX_MASK;
		}
		else
		{
			/*Set as receiver*/
			I2C1->C1 &= ~I2C_C1_TX_MASK;
		}
	break;
	case(I2C_2):
		if(TRANSMITTER == tx_or_rx)
		{
			/*Set as transmitter*/
			I2C2->C1 |= I2C_C1_TX_MASK;
		}
		else
		{
			/*Set as receiver*/
			I2C2->C1 &= ~I2C_C1_TX_MASK;
		}
	break;
	}
}

void I2C_nack(i2c_channel_t channel)
{
	switch(channel){
	case(I2C_0):
		I2C0->C1 |= I2C_C1_TXAK(TRUE);
		break;
	case(I2C_1):
		I2C1->C1 |= I2C_C1_TXAK(TRUE);
		break;
	case(I2C_2):
		I2C2->C1 |= I2C_C1_TXAK(TRUE);
		break;
	}
}

void I2C_repeted_start(i2c_channel_t channel)
{
	switch(channel){
	case(I2C_0):
		I2C0->C1 |= I2C_C1_RSTA(TRUE);
		break;
	case(I2C_1):
		I2C1->C1 |= I2C_C1_RSTA(TRUE);
		break;
	case(I2C_2):
		I2C2->C1 |= I2C_C1_RSTA(TRUE);
		break;
	}
}

void I2C_write_byte(i2c_channel_t channel, uint8_t data)
{
	switch(channel){
	case(I2C_0):
		I2C0->D = data;
		break;
	case(I2C_1):
		I2C1->D = data;
		break;
	case(I2C_2):
		I2C2->D = data;
		break;
	}
}

uint8_t  I2C_read_byte(i2c_channel_t channel)
{
	uint8_t I2C_data;
	switch(channel){
	case(I2C_0):
		I2C_data = I2C0->D;
		break;
	case(I2C_1):
		I2C_data = I2C1->D;
		break;
	case(I2C_2):
		I2C_data = I2C2->D;
		break;
	}
	return I2C_data;
}

void I2C_wait(i2c_channel_t channel)
{
	switch(channel){
	case(I2C_0):
		while(0 == (I2C0->S & I2C_S_IICIF_MASK));
		I2C0->S |= I2C_S_IICIF_MASK;
		break;
	case(I2C_1):
		while(0 == (I2C1->S & I2C_S_IICIF_MASK));
		I2C1->S |= I2C_S_IICIF_MASK;
		break;
	case(I2C_2):
		while(0 == (I2C2->S & I2C_S_IICIF_MASK));
		I2C2->S |= I2C_S_IICIF_MASK;
		break;
	}
}

uint8_t I2C_get_ack(i2c_channel_t channel)
{
	volatile uint8_t ack_status = FALSE;
	switch(channel){
	case(I2C_0):
		ack_status = (I2C0->S & I2C_S_RXAK_MASK);
		break;
	case(I2C_1):
		ack_status = (I2C1->S & I2C_S_RXAK_MASK);
		break;
	case(I2C_2):
		ack_status = (I2C2->S & I2C_S_RXAK_MASK);
		break;
	}
	return ack_status;
}

void I2C_start(i2c_channel_t channel)
{
	switch(channel){
	case(I2C_0):
		/** Set as master and transmitter mode*/
		I2C_mst_or_slv_mode(I2C_0, MASTER);
		I2C_tx_rx_mode(I2C_0, TRANSMITTER);
		break;
	case(I2C_1):
		/** Set as master and transmitter mode*/
		I2C_mst_or_slv_mode(I2C_1, MASTER);
		I2C_tx_rx_mode(I2C_1, TRANSMITTER);
		break;
	case(I2C_2):
		/** Set as master and transmitter mode*/
		I2C_mst_or_slv_mode(I2C_2, MASTER);
		I2C_tx_rx_mode(I2C_2, TRANSMITTER);
		break;
	}
}

void I2C_stop(i2c_channel_t channel)
{
	switch(channel){
	case(I2C_0):
		//I2C0->C1 &= ~I2C_C1_TXAK_MASK;
		I2C_mst_or_slv_mode(I2C_0, SLAVE);
		I2C_tx_rx_mode(I2C_0, RECEIVER);
		break;
	case(I2C_1):
		//I2C0->C1 &= ~I2C_C1_TXAK_MASK;
		I2C_mst_or_slv_mode(I2C_1, SLAVE);
		I2C_tx_rx_mode(I2C_1, RECEIVER);
		break;
	case(I2C_2):
		//I2C0->C1 &= ~I2C_C1_TXAK_MASK;
		I2C_mst_or_slv_mode(I2C_2, SLAVE);
		I2C_tx_rx_mode(I2C_2, RECEIVER);
		break;
	}
}
