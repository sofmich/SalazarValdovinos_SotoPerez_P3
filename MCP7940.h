/**
	\file	MCP7940.h
	\brief
		This is the API for manage of a Real Time Converter RTC MCP7940 using kinetis
		FRDM K64. It runs all the implementation configuration to set and read a
		time with format HH:MM:SS
	\author Sofia Salazar, Omar Soto
	\date	18/04/2020
	\todo
	    Other functionalities from RTC are not implemented on this API.
 */
#ifndef MCP7940_H_
#define MCP7940_H_

#include "MK64F12.h"
#include "bits.h"
#include "I2C.h"

/** Bit of control to generate a wave of 1Hz*/
#define ST			0x80
/** Byte of control to communicate through I2C protocol*/
#define CONTROL		0x07
/** Byte of control to write on RTC*/
#define AD_WRITE 	0xDE
/** Byte of control to read on RTC*/
#define AD_READ 	0XDF
/** Content to generate a wave of 1Hz*/
#define MFT			0x40
/** Address of seconds on RTC*/
#define SEC 		0x00
/** Address of minutes on RTC*/
#define MIN 		0x01
/** Address of hours on RTC*/
#define HOUR 		0x02
/** Mask to get tens of hours*/
#define TENS_HOURS		30U
/** Mask to get tens of minutes*/
#define TENS_MINS		0x070U
/** Mask to get tens of seconds*/
#define TENS_SEC		0x070U
/** Mask to shift the tens since they are in the high offset*/
#define CENTS_SHIFTER	4U
/** Mask to get units of byte */
#define UNITS_MASK		0x0FU
/** Mask to get wheter is AM or PM*/
#define AM_PM_MASK		0x020

/** ASCII constant that will be substracted or added*/
#define ASCII_CONST	48U
/** Constan to represent ':' dots*/
#define ASCII_DOTS	58U
/** select whether the hour is half day or past half day*/
typedef enum { AM, PM} am_pm_t;

/** struct to save tens and units of elements on hour format*/
typedef struct
{
	uint8_t tens_hours;
	uint8_t units_hours;
	uint8_t	tens_minutes;
	uint8_t units_minutes;
	uint8_t tens_seconds;
	uint8_t units_seconds;
	uint8_t am_pm;

}time_format_t;

/** Constants indicate tens and units of elements on hour format*/
typedef enum{
	HOURS_TENS,
	HOURS_UNITS,
	MINUTES_TENS,
	MINUTES_UNITS,
	SECONDS_TENS,
	SECONDS_UNITS
}time_elements_t;

/********************************************************************************************/
/*!
 	 \brief	Intialize all necessary bytes nformation and I2C protocol.
 	 \param[in]  I2C channel to be used
 	 \param[in]  I2C baudrate
 	 \return void
 */
void Init_MCP7940(i2c_channel_t channel, i2c_baud_rate_t baudrate);

/********************************************************************************************/
/*!
 	 \brief	Set a new second value
 	 \param[in]  seconds vlaue
 	 \return void
 */
void SetSecond(uint8_t sec);
/********************************************************************************************/
/*!
 	 \brief	Set a new minute value
 	 \param[in]  minutes value
 	 \return void
 */
void SetMinute(uint8_t min);
/********************************************************************************************/
/*!
 	 \brief	Set a new hour value
 	 \param[in]  hours value
 	 \return void
 */
void SetHour(uint8_t hour);
/********************************************************************************************/
/*!
 	 \brief	Allows RTC to start generating a 1HZ signal and start working communicating
 	 	 	with I2C from kinetis.
 	 \param[in]  void
 	 \return void
 */
void RTCenable(void);
/********************************************************************************************/
/*!
 	 \brief	Get the actual seconds value
 	 \param[in]  void
 	 \return new seconds value
 */
uint8_t GetSecond(void);
/********************************************************************************************/
/*!
 	 \brief	Get the actual mintes value
 	 \param[in]  void
 	 \return new minutes value
 */
uint8_t GetMinute(void);
/********************************************************************************************/
/*!
 	 \brief	Get the actual hours value
 	 \param[in]  void
 	 \return new hours value
 */
uint8_t GetHour(void);
/********************************************************************************************/
/*!
 	 \brief	Update the new time value on UART to let the digital clock be seen by the user
 	 \param[in]  void
 	 \return void
 */
void UpdateDisplayTime(void);
/********************************************************************************************/
/*!
 	 \brief	Set a new time value acording to format HH:MM:SS
 	 \param[in]  array containing all the values as numeric form
 	 \return void
 */
void UpdateTime(uint8_t time[]);


#endif /* MCP7940_H_ */
