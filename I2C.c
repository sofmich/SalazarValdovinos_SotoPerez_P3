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

i2c_baud_rate_t g_baud_rate;

void I2C_init(i2c_channel_t channel, uint32_t system_clock, i2c_baud_rate_t baud_rate)
{

	switch(channel)
	{
	case(I2C_0):
			{
		/** Enable the clock gating for I2C0 */
		SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK; /*Enables the I2C0 Clock Gating*/
		/** Assign the Mult value and the ICR value*/
		I2C0->F |= 	I2C_F_MULT(g_baud_rate.mult);
		I2C0->F |=  I2C_F_ICR(g_baud_rate.icr);

		/** An Acknowledge Not-Acknowledge can be sent **/
		I2C0->SMB |= I2C_SMB_FACK_MASK;
		/** Enable the I2C0 module*/
		I2C0->C1 |= I2C_C1_IICEN_MASK;

		const gpio_pin_control_register_t pinControlRegisterGPIOB = GPIO_MUX2;
		/**Enable clock from pin*/
		GPIO_clock_gating(GPIO_B);
		/** Enable PCR clock gating of I2C pines*/
		GPIO_pin_control_register(GPIO_B,bit_2,&pinControlRegisterGPIOB);
		GPIO_pin_control_register(GPIO_B,bit_3,&pinControlRegisterGPIOB);

			}
	break;
	case(I2C_1):
			{
		/** Enable the clock gating for I2C0 */
		SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK; /*Enables the I2C0 Clock Gating*/
		/** Assign the Mult value and the ICR value*/
		I2C1->F |= g_baud_rate.mult | g_baud_rate.icr;
		/** An Acknowledge Not-Acknowledge can be sent **/
		I2C1->SMB |= I2C_SMB_FACK_MASK;
		/** Enable the I2C0 module*/
		I2C1->C1 |= I2C_C1_IICEN_MASK;
			}
	break;
	case(I2C_2):
			{
		/** Enable the clock gating for I2C0 */
		SIM->SCGC1 |= SIM_SCGC1_I2C2_MASK; /*Enables the I2C0 Clock Gating*/
		/** Assign the Mult value and the ICR value*/
		I2C2->F |= g_baud_rate.mult | g_baud_rate.icr;
		/** An Acknowledge Not-Acknowledge can be sent **/
		I2C2->SMB |= I2C_SMB_FACK_MASK;
		/** Enable the I2C0 module*/
		I2C2->C1 |= I2C_C1_IICEN_MASK;
			}
	break;
	}
}

void I2C_mst_or_slv_mode(master_or_slave_t mst_or_slv)
{
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
}

void I2C_tx_rx_mode(uint8_t tx_or_rx)
{
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
}

void I2C_nack(void)
{
	I2C0->C1 |= I2C_C1_TXAK(TRUE);
}

void I2C_repeted_start(void)
{
	I2C0->C1 |= I2C_C1_RSTA(TRUE);
}

void I2C_write_byte(uint8_t data)
{
	I2C0->D = data;
}

uint8_t  I2C_read_byte(void)
{
	uint8_t I2C_data;
	I2C_data = I2C0->D;
	return I2C_data;
}

void I2C_wait(void)
{
	while(0 == (I2C0->S & I2C_S_IICIF_MASK));
	I2C0->S |= I2C_S_IICIF_MASK;
}

uint8_t I2C_get_ack(void)
{
	volatile uint8_t ack_status = FALSE;
	ack_status = (I2C0->S & I2C_S_RXAK_MASK);
	return ack_status;
}

void I2C_start(void)
{
	/** Set as master and transmitter mode*/
	I2C_mst_or_slv_mode(MASTER);
	I2C_tx_rx_mode(TRANSMITTER);
}

void I2C_stop(void)
{
	//I2C0->C1 &= ~I2C_C1_TXAK_MASK;
	I2C_mst_or_slv_mode(SLAVE);
	I2C_tx_rx_mode(RECEIVER);
}
