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
/** Adress of days on RTC*/
#define DAYS_ADDRESS 	0x04
/** Adress of months on RTC*/
#define MONTHS_ADDRESS 	0x05
/** Adress of years on RTC*/
#define YEARS_ADDRESS 	0x06
/** Mask to get tens of hours*/
#define TENS_HOURS		0x30U
/** Mask to get tens of minutes*/
#define TENS_MINS		0x070U
/** Mask to get tens of seconds*/
#define TENS_SEC		0x070U
/** Mask to shift the tens since they are in the high offset*/
#define CENTS_SHIFTER	4U
/** Mask to get units of byte */
#define UNITS_MASK		0x0FU
/** Mask to get cents of years*/
#define YEARS_CENTS_MASK	0xF0
/** Mask to get cents of months*/
#define MONTHS_CENTS_MASK	0x10
/** Mask to get cents of days*/
#define DAYS_CENTS_MASK		0x30
/** Mask to get wheter is AM or PM*/
#define AM_PM_MASK		0x020

/** ASCII constant that will be substracted or added*/
#define ASCII_CONST	48U
/** Constan to represent ':' dots*/
#define ASCII_DOTS	58U
/** select whether the hour is half day or past half day*/
typedef enum { AM, PM} am_pm_t;
/** Data type to recognize if a date or time is being configured*/
typedef uint8_t config_rtc_t ;

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

/** struct to save tens and units of elements on date format*/
typedef struct
{
	uint8_t tens_years;
	uint8_t units_years;
	uint8_t	tens_months;
	uint8_t units_months;
	uint8_t tens_days;
	uint8_t units_days;
}date_format_t;

/** Constants indicate tens and units of elements on hour format*/
typedef enum{
	HOURS_TENS,
	HOURS_UNITS,
	MINUTES_TENS,
	MINUTES_UNITS,
	SECONDS_TENS,
	SECONDS_UNITS
}time_elements_t;

typedef enum{
	NONE,
	TIME,
	DATE,
	HOURS,
	MINUTES,
	SECONDS,
	DAY,
	MONTH,
	YEAR
}rtc_elements_t;

typedef enum
{
	mode_12_hrs,
	mode_24_hrs,
}format_t;

/********************************************************************************************/
/*!
 	 \brief	Intialize all necessary bytes nformation and I2C protocol.
 	 \param[in]  I2C channel to be used
 	 \param[in]  I2C baudrate
 	 \return void
 */
void Init_MCP7940(void);

/********************************************************************************************/
/*!
 	 \brief	Write a new value on RTC
 	 \param[in]  byte to write
 	 \return void
 */
void RTC_write_byte(uint8_t address, uint8_t data);
/********************************************************************************************/
/*!
 	 \brief	READ a byte on RTC
 	 \param[in]  address to read
 	 \return data
 */
uint8_t RTC_read_byte(uint8_t address);

/********************************************************************************************/
/*!
 	 \brief	Set a new time value acording to format HH:MM:SS
 	 \param[in]  array containing all the values as numeric form
 	 \return void
 */
void UpdateTime(config_rtc_t date_or_time, uint8_t time[]);
/********************************************************************************************/
/*!
 	 \brief	Get the actual value of time
 	 \param[in]  void
 	 \return estructure with all digits
 */
time_format_t GetGlobalTime(void);
/********************************************************************************************/
/*!
 	 \brief	Get the actual value of date
 	 \param[in]  void
 	 \return direction of date values
 */

date_format_t GetGlobalDate(void);
/********************************************************************************************/
/*!
 	 \brief	Set time to 12/24 hrs
 	 \param[in]  mode
 	 \return void
 */
void setFormat(format_t mode);

#endif /* MCP7940_H_ */
