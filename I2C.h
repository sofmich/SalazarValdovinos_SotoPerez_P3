/**
	\file
	\brief
		This is the header file for the I2C device driver.
		i.e., this is the application programming interface (API) for the I2C peripheral.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	3/04/2020
	\todo
	    Functions are not implemented.
 */

#ifndef I2C_H_
#define I2C_H_

#include "MK64F12.h"
#include "stdint.h"
#include "bits.h"


/** Constant that represent the clock enable for GPIO A */
#define I2C0_CLOCK_GATING 0x40
/**
 * \brief This enum defines the I2C channel to be used.
 */
typedef enum {I2C_0, I2C_1, I2C_2} i2c_channel_t;


/**
 * \brief This bilf field is used to configure the the I2C baud rate.
 */
typedef struct
{
	uint8_t icr  : 6;
	uint8_t mult : 2;
} i2c_baud_rate_t;

typedef struct
{
	i2c_channel_t channel;
	uint32_t system_clock;
	uint8_t multiplier;
	uint8_t clock_rate;

}i2c_config_struct_t;


/**
 * \brief This constants are used to configure the the I2C slave/master mode.
 */
typedef enum {SLAVE, MASTER} master_or_slave_t;


/**
 * \brief This constants are used to configure the the I2C tx/rx mode.
 */
typedef enum {RECEIVER, TRANSMITTER} tx_or_rx_t;


/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 Configures the I2C port based on the input parameters ans enables the I2C module
  	 	 Also, internally this function configures the GPIO, pin control register and clock gating, to be used as I2C.
  	 	 It is recommended to use pin 2 and 3 of GPIOB.
  	 \param[in] channel It is the channel to be used.
  	 \param[in] systemClock Frequency of the system.
  	 \param[in] baud_rate is ans structure that contains the fiels mult and icr.
  	 \return void
  */
void I2C_init(i2c_config_struct_t *config);

/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 Configures the I2C port based on the input parameters ans enables the I2C module.
  	 \param[in] channel to activate clock
  	 \return void
  */
void I2C_clock_gating(i2c_channel_t channel);
/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 Configures the I2C port baud rate based on the input parameters ans enables the I2C module
  	 \param[in] channel It is the channel to be used.
  	 \param[in] baud_rate is ans structure that contains the fiels mult and icr.
  	 \param[in] systemClock Frequency of the system.
  	 \return void
  */
void I2C_baud_rate(i2c_channel_t channel ,uint8_t multiplier, uint8_t clock_rate);
/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 Configures the I2C port baud rate based on the input parameters ans enables the I2C module
  	 \param[in] channel It is the channel to be used.
  	 \return void
  */
void I2C_enable(i2c_channel_t channel);

 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 It selects between master or slave mode by modifying bit MST.
  	 	 This function is called inside I2C_start and I2C_stop
  	 \param[in] master_or_slave If == 1 master mode, if == 0 slave mode.
  	 \return void
  */
 void I2C_mst_or_slv_mode(i2c_channel_t channel, uint8_t mst_or_slv);
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 It selects between transmitter mode or receiver mode.
  	 	 It selects between master or slave mode by modifying bit TX.
  	 	 This function is called inside I2C_start and I2C_stop
  	 \param[in] tx_or_rx If == 1 transmitter mode, if == 0 slave mode.
  	 \return void
  */
 void I2C_tx_rx_mode(i2c_channel_t channel, uint8_t tx_or_rx);
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 It generates the Not ACKnowledge that is needed when the master reads data.
  	 	 It modifies  TXAK bit
  	 \return void
  */
 void I2C_nack(i2c_channel_t channel);
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 It generates a repeated start that is needed when master reads data.
  	 	 Ii modifies RSTA bit.
  	 \return void
  */
 void I2C_repeted_start(i2c_channel_t channel);
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 It writes the data to be transmitted into the transmission buffer. When you want to
  	 	 write a value into the buffer you need to use this sentence I2C0_D = data. Avoid to use
  	 	 masks like this I2C0_D |= data.
  	 \return void
  */
void I2C_write_byte(i2c_channel_t channel, uint8_t data);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 It reads data from the receiving buffer.
 	 \return void
 */
uint8_t  I2C_read_byte(i2c_channel_t channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Indicates the status of the bus regardless of slave or master mode. Internally checks the interrupt flag of the
 	 	 I2Cx_S register. This bit is set on the following events:
			• One byte transfer, including ACK/NACK bit, completes if FACK is 0. An ACK or NACK is sent on the
			  bus by writing 0 or 1 to TXAK after this bit is set in receive mode.
			• One byte transfer, excluding ACK/NACK bit, completes if FACK is 1.
		This function should be implemented as a blocking function by using  while((I2C0->S & 0x02)== 0);, the bit number 2 of this register must be set.
		The blocking implementation of this function only to reduce the complexity of the lab. However, blocking implementation must be avoided.
 	 \return Void.
 */
void I2C_wait(i2c_channel_t channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Indicates if the acknowledge was received.
 	 	 It checks RXAK bit
 	 \return This function returns a 0 logic if the acknowledge was received and returns 1 logic if the acknowledge was not received.
 */
uint8_t I2C_get_ack(i2c_channel_t channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Generates the start signal. When MST bit is changed from 0 to 1, a START signal is generated 
 	 	 on the bus and master mode is selected. Also, inside the function the I2C is
 	 	 change to transmitter mode.
 	 \return void
 */
void I2C_start(i2c_channel_t channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Generates the stop signal. When this bit changes from 1 to 0, a STOP signal is generated 
 	 	 and the mode of operation changes from master to slave. Also, inside the function the I2C is
 	 	 change to receiver mode.
 	 \return void
 */
void I2C_stop(i2c_channel_t channel);

#endif /* I2C_H_ */
