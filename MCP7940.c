/**
	\file
	\brief
		This is the API for manage of the RTC MCP7940.
	\author Omar Soto, Sofia Salazar
	\date	05/04/2020
	\todo
 */

#include "MCP7940.h"
#include "I2C.h"
#include "GPIO.h"
#include "bits.h"
#include "NVIC.h"
#include "UART.h"
#include "delay.h"
#include "system_status.h"

#define SYSTEM_CLOCK (21000000)
#define DELAY 100U

uint8_t g_Status_RTC = FALSE;

/** Pointer to function to execute function from system_status*/
void (*ptr_SystemError_RTC)(uint8_t)=SYSTEM_I2C_fail;

/** Configuration for I2C baud rate*/
const i2c_baud_rate_t RTC_baud_rate = {2, 1 };

static const i2c_config_struct_t RTC_config =
{
		I2C_0, /** Channel*/
		21000000, /** System clock*/
		RTC_baud_rate.mult, /** Multiplier*/
		RTC_baud_rate.icr, /** Multiplier*/
};

void Init_MCP7940(){
	/** Start comunicating with the RTC*/
	I2C_init(&RTC_config);
	/** Initialization hour*/
	uint8_t init_hour[] = {0,0,0,0,0,0};
	/** Set the init hour*/
	UpdateTime(TIME, init_hour);
	/** Generate a 1Hz Clock*/
	RTCenable();
}

void RTCenable(void){
	I2C_tx_rx_mode(I2C_0,TRANSMITTER);
	I2C_start(I2C_0);

	I2C_write_byte(I2C_0,AD_WRITE);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0,CONTROL);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0,MFT);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);
	I2C_stop(I2C_0);
}

void RTC_write_byte(uint8_t address, uint8_t data)
{
	uint8_t ackError = FALSE;

	I2C_tx_rx_mode(I2C_0,TRANSMITTER);
	I2C_start(I2C_0);

	I2C_write_byte(I2C_0,AD_WRITE);
	I2C_wait(I2C_0);
	ackError = I2C_get_ack(I2C_0);

	if(TRUE == ackError)
	{
		ptr_SystemError_RTC(g_Status_RTC);
		I2C_stop(I2C_0);
	}
	else{

		I2C_write_byte(I2C_0,address);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);

		I2C_write_byte(I2C_0,data);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);
		I2C_stop(I2C_0);

	}

}
uint8_t RTC_read_byte(uint8_t address)
{
	uint8_t data;
	uint8_t ackError = FALSE;

	I2C_tx_rx_mode(I2C_0,TRANSMITTER);
	I2C_start(I2C_0);

	I2C_write_byte(I2C_0,AD_WRITE);
	I2C_wait(I2C_0);
	ackError = I2C_get_ack(I2C_0);
	if(TRUE == ackError)
	{
		ptr_SystemError_RTC(g_Status_RTC);
		I2C_stop(I2C_0);
	}
	else{
		I2C_write_byte(I2C_0,address);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);

		I2C_repeted_start(I2C_0);
		I2C_write_byte(I2C_0,AD_READ);
		I2C_wait(I2C_0);
		I2C_get_ack(I2C_0);

		I2C_tx_rx_mode(I2C_0,RECEIVER);

		I2C_nack(I2C_0);
		data = I2C_read_byte(I2C_0);
		I2C_wait(I2C_0);
		I2C_nack(I2C_0);

		I2C_stop(I2C_0);
		data = I2C_read_byte(I2C_0);

		return data;
	}
}



void UpdateTime(config_rtc_t date_or_time, uint8_t time[])
{
	/**Time or date comes in format  CENTS UNITS/CENTS UNITS/CENTS/UNITS  so let's get a byte to send*/
	uint8_t sec = (time[SECONDS_TENS] << CENTS_SHIFTER) | (time[SECONDS_UNITS]);
	uint8_t min = (time[MINUTES_TENS] << CENTS_SHIFTER) | (time[MINUTES_UNITS]);
	uint8_t hor = (time[HOURS_TENS] << CENTS_SHIFTER) | (time[	HOURS_UNITS]);
	switch(date_or_time)
	{
	case(TIME):
	{
		g_Status_RTC = SET_TIME;
		/** Allows to set a new hour*/
		RTC_write_byte(HOUR, hor);
		/** Give time to I2C to finish transfer*/
		delay(DELAY);
		/** Allows to set a new minutes value*/
		RTC_write_byte(MIN, min);
		/** Give time to I2C to finish transfer*/
		delay(DELAY);
		/** Allows to set a new seconds value*/
		RTC_write_byte(SEC, (ST |sec));
		/** Give time to I2C to finish transfer*/
		delay(DELAY);
	}
		break;
	case(DATE):
	{
		g_Status_RTC = SET_DATE;
		/** Allows to set a new DAY*/
		RTC_write_byte(DAYS_ADDRESS, hor);
		/** Give time to I2C to finish transfer*/
		delay(DELAY);
		/** Allows to set a new month*/
		RTC_write_byte(MONTHS_ADDRESS, min);
		/** Give time to I2C to finish transfer*/
		delay(DELAY);
		/** Allows to set a new DAY*/
		RTC_write_byte(YEARS_ADDRESS, sec);
		/** Give time to I2C to finish transfer*/
		delay(DELAY);

	}
	break;

	}

}

time_format_t GetGlobalTime(void)
{
	g_Status_RTC =  READ_TIME;
	/** Structure to save all elements of time format*/
	time_format_t actual_time;
	/** Get hours and print them on UART*/
	uint8_t temp_time = RTC_read_byte(HOUR);
	delay(DELAY);
	/** Get hours tens*/
	actual_time.tens_hours = (temp_time & TENS_HOURS) >> CENTS_SHIFTER;
	/** Get hours units*/
	actual_time.units_hours = temp_time & UNITS_MASK;
	/** Set AM or PM time*/
	temp_time &= AM_PM_MASK ;
	temp_time = (temp_time >> bit_5);
	actual_time.am_pm = temp_time;
	/** Get minutes and print them on UART*/
	temp_time = RTC_read_byte(MIN);
	delay(DELAY);
	/** Get minutes teens*/
	actual_time.tens_minutes = (temp_time & TENS_MINS) >>  CENTS_SHIFTER;
	/** Get minutes units*/
	actual_time.units_minutes = temp_time & UNITS_MASK;
	/** Get seconds and print them on UART*/
	temp_time = RTC_read_byte(SEC);
	delay(DELAY);
	/** Get seconds teens*/
	actual_time.tens_seconds = (temp_time & TENS_SEC) >> CENTS_SHIFTER;
	/** Get seconds units*/
	actual_time.units_seconds = temp_time & UNITS_MASK;

	return actual_time;
}

date_format_t GetGlobalDate(void){
	g_Status_RTC =  READ_DATE;
	uint8_t date_format[3];
	date_format_t actual_date = {FALSE};
	/** Read all digits of date*/
	date_format[0] = RTC_read_byte(YEARS_ADDRESS);
	date_format[1] = RTC_read_byte(MONTHS_ADDRESS);
	date_format[2] = RTC_read_byte(DAYS_ADDRESS);

	/** Adjust tens and units of each digit*/
	actual_date.tens_years = (date_format[0] & YEARS_CENTS_MASK) >> CENTS_SHIFTER;
	actual_date.units_years = (date_format[0] & UNITS_MASK);
	/* Adjust tens and units of months*/
	actual_date.tens_months = (date_format[1] & MONTHS_CENTS_MASK) >> CENTS_SHIFTER;
	actual_date.units_months = (date_format[1] & UNITS_MASK);
	/* Adjust tens and units of days*/
	actual_date.tens_days = (date_format[2] & DAYS_CENTS_MASK) >>  CENTS_SHIFTER;
	actual_date.units_days = (date_format[2] & UNITS_MASK);
	return actual_date;
}

void setFormat(format_t mode)
{
	uint8_t set_12_24 = RTC_read_byte(YEARS_ADDRESS);
	switch(mode)
	{
	case(mode_12_hrs):
		set_12_24 |= (TRUE << bit_6);
			break;
	case(mode_24_hrs):
			set_12_24 |= (FALSE << bit_6);
	break;
	}
	RTC_write_byte(HOUR, set_12_24);
	delay(DELAY);
}
