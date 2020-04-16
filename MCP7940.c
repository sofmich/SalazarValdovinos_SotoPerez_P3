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

#define SYSTEM_CLOCK (21000000)
#define DELAY 100U

void Init_MCP7940(i2c_channel_t channel,  i2c_baud_rate_t baudrate){
	/** Start comunicating with the RTC*/
	I2C_init(channel, SYSTEM_CLOCK, baudrate);
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
	I2C_tx_rx_mode(I2C_0,TRANSMITTER);
	I2C_start(I2C_0);

	I2C_write_byte(I2C_0,AD_WRITE);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0,address);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

	I2C_write_byte(I2C_0,data);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);
	I2C_stop(I2C_0);

}
uint8_t RTC_read_byte(uint8_t address)
{
	uint8_t data;
	I2C_tx_rx_mode(I2C_0,TRANSMITTER);
	I2C_start(I2C_0);

	I2C_write_byte(I2C_0,AD_WRITE);
	I2C_wait(I2C_0);
	I2C_get_ack(I2C_0);

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


void UpdateDisplayTime(void)
{
	/** Structure to save all elements of time format*/
	time_format_t actual_time;
	/** Get hours and print them on UART*/
	uint8_t temp_time = RTC_read_byte(HOUR);
	delay(DELAY);
	printf("==============TEMP TIME HRS %x \n", temp_time);
	if(temp_time < 12)
	{
		actual_time.am_pm = AM;
	}
	else
	{
		temp_time = temp_time - 12;
		actual_time.am_pm = PM;
	}
	/** Get hours tens*/
	actual_time.tens_hours = (temp_time & TENS_HOURS) >> CENTS_SHIFTER;
	printf("********* HRS TO PRINT %x \n", actual_time.tens_hours);
	/** Get hours units*/
	actual_time.units_hours = temp_time & UNITS_MASK;
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

	printf("Actual time \n %d%d : %d%d: %d%d AM O PM %d\n",
			actual_time.tens_hours, actual_time.units_hours,
			actual_time.tens_minutes, actual_time.units_minutes,
			actual_time.tens_seconds, actual_time.units_seconds,
			actual_time.am_pm);

	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[12;10H");
	/** Print chars of display time HH:MM:SS am*/
	uint8_t dots = ASCII_DOTS;
	UART_put_char(UART_0, actual_time.tens_hours + ASCII_CONST);
	UART_put_char(UART_0, actual_time.units_hours + ASCII_CONST);
	UART_put_char(UART_0, dots);
	UART_put_char(UART_0, actual_time.tens_minutes+ ASCII_CONST);
	UART_put_char(UART_0, actual_time.units_minutes + ASCII_CONST);
	UART_put_char(UART_0, dots);
	UART_put_char(UART_0, actual_time.tens_seconds + ASCII_CONST);
	UART_put_char(UART_0, actual_time.units_seconds + ASCII_CONST);
	switch(actual_time.am_pm)
	{
	case(AM):
		UART_put_string(UART_0,"   AM");
	break;
	case(PM):
		UART_put_string(UART_0,"   AM");
	break;
	}


}
void UpdateDisplayDate(void)
{
	uint8_t date_format[3];
	date_format_t actual_date = {FALSE};
	/** Read all digits of date*/
	date_format[0] = RTC_read_byte(YEARS_ADDRESS);
	date_format[1] = RTC_read_byte(MONTHS_ADDRESS);
	date_format[2] = RTC_read_byte(DAYS_ADDRESS);
	printf("++++++++++ Actual date\n %x / %x / %x \n",
			date_format[2], date_format[1], date_format[0]);

	/** Adjust tens and units of each digit*/
	actual_date.tens_years = (date_format[0] & YEARS_CENTS_MASK) >> CENTS_SHIFTER;
	actual_date.units_years = (date_format[0] & UNITS_MASK);
	/* Adjust tens and units of months*/
	actual_date.tens_months = (date_format[1] & MONTHS_CENTS_MASK) >> CENTS_SHIFTER;
	actual_date.units_months = (date_format[1] & UNITS_MASK);
	/* Adjust tens and units of days*/
	actual_date.tens_days = (date_format[2] & DAYS_CENTS_MASK) >>  CENTS_SHIFTER;
	actual_date.units_days = (date_format[2] & UNITS_MASK);

	/** VT100 command for positioning the cursor in x and y position*/
	UART_put_string(UART_0,"\033[12;10H");
	/** Print chars of display time HH:MM:SS am*/
	UART_put_char(UART_0, actual_date.tens_days + ASCII_CONST);
	UART_put_char(UART_0, actual_date.units_days + ASCII_CONST);
	UART_put_char(UART_0, '/');
	UART_put_char(UART_0, actual_date.tens_months + ASCII_CONST);
	UART_put_char(UART_0, actual_date.units_months  + ASCII_CONST);
	UART_put_char(UART_0, '/');
	UART_put_char(UART_0, actual_date.tens_years + ASCII_CONST);
	UART_put_char(UART_0, actual_date.units_years  + ASCII_CONST);
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
