/**
	\file
	\brief
		This is the API for manage of the RTC MCP7940.
	\author Omar Soto, Sofia Salazar
	\date	05/04/2020
	\todo
		To implement the UART_init function
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
	UpdateTime(init_hour);
	/** Generate a 1Hz Clock*/
	RTCenable();
}

void RTCenable(void){
	I2C_tx_rx_mode(TRANSMITTER);
	I2C_start();

	I2C_write_byte(AD_WRITE);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(CONTROL);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(MFT);
	I2C_wait();
	I2C_get_ack();
	I2C_stop();
}

void SetSecond(uint8_t sec){
	I2C_tx_rx_mode(TRANSMITTER);
	I2C_start();

	I2C_write_byte(AD_WRITE);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(SEC);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(ST | sec);
	I2C_wait();
	I2C_get_ack();
	I2C_stop();
}

void SetMinute(uint8_t min){
	I2C_tx_rx_mode(TRANSMITTER);
	I2C_start();

	I2C_write_byte(AD_WRITE);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(MIN);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(min);
	I2C_wait();
	I2C_get_ack();
	I2C_stop();
}

void SetHour(uint8_t hour){
	I2C_tx_rx_mode(TRANSMITTER);
	I2C_start();

	I2C_write_byte(AD_WRITE);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(HOUR);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(hour);
	I2C_wait();
	I2C_get_ack();
	I2C_stop();
}

uint8_t GetSecond(void){
	uint8_t data;
	I2C_tx_rx_mode(TRANSMITTER);
	I2C_start();

	I2C_write_byte(AD_WRITE);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(SEC);
	I2C_wait();
	I2C_get_ack();

	I2C_repeted_start();
	I2C_write_byte(AD_READ);
	I2C_wait();
	I2C_get_ack();

	I2C_tx_rx_mode(RECEIVER);

	I2C_nack();
	data = I2C_read_byte();
	I2C_wait();
	I2C_nack();

	I2C_stop();
	data = I2C_read_byte();

	return data;
}

uint8_t GetMinute(void){
	uint8_t data;

	I2C_tx_rx_mode(TRANSMITTER);
	I2C_start();

	I2C_write_byte(AD_WRITE);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(MIN);
	I2C_wait();
	I2C_get_ack();

	I2C_repeted_start();
	I2C_write_byte(AD_READ);
	I2C_wait();
	I2C_get_ack();

	I2C_tx_rx_mode(RECEIVER);

	I2C_nack();
	data = I2C_read_byte();
	I2C_wait();
	I2C_nack();

	I2C_stop();
	data = I2C_read_byte();

	return data;
}

uint8_t GetHour(void){

	uint8_t data;

	I2C_tx_rx_mode(TRANSMITTER);
	I2C_start();

	I2C_write_byte(AD_WRITE);
	I2C_wait();
	I2C_get_ack();

	I2C_write_byte(HOUR);
	I2C_wait();
	I2C_get_ack();

	I2C_repeted_start();
	I2C_write_byte(AD_READ);
	I2C_wait();
	I2C_get_ack();

	I2C_tx_rx_mode(RECEIVER);

	I2C_nack();
	data = I2C_read_byte();
	I2C_wait();
	I2C_nack();

	I2C_stop();
	data = I2C_read_byte();
	return data;
}

void UpdateDisplayTime(void)
{
	/** Structure to save all elements of time format*/
	time_format_t actual_time;
	/** Get hours and print them on UART*/
	uint8_t temp_time = GetHour();
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
	temp_time = GetMinute();
	delay(DELAY);
	/** Get minutes teens*/
	actual_time.tens_minutes = (temp_time & TENS_MINS) >>  CENTS_SHIFTER;
	/** Get minutes units*/
	actual_time.units_minutes = temp_time & UNITS_MASK;
	/** Get seconds and print them on UART*/
	temp_time = GetSecond();
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
	UART_put_string(UART_0,"\033[11;10H");
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

void UpdateTime(uint8_t time[])
{

	/**Time comes in format  DH UH  DM UM   DS US so let's get a byte to send*/
	uint8_t sec = (time[SECONDS_TENS] << CENTS_SHIFTER) | (time[SECONDS_UNITS]);
	uint8_t min = (time[MINUTES_TENS] << CENTS_SHIFTER) | (time[MINUTES_UNITS]);
	uint8_t hor = (time[HOURS_TENS] << CENTS_SHIFTER) | (time[	HOURS_UNITS]);
	/** Allows to set a new hour*/
	SetHour(hor);
	/** Give time to I2C to finish transfer*/
	delay(DELAY);
	/** Allows to set a new minutes value*/
	SetMinute(min);
	/** Give time to I2C to finish transfer*/
	delay(DELAY);
	/** Allows to set a new seconds value*/
	SetSecond(sec);
	/** Give time to I2C to finish transfer*/
	delay(DELAY);
}


