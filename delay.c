/**
	\file	delay.c
	\brief
		This module makes a delay using a for loop.
	\author Sofía Salazar
	\date	13/04/2019
	\todo
 */

#include "MK64F12.h" /* include peripheral declarations */

void delay(uint32_t delay)
{
	volatile uint32_t j, i;

	for(j = delay; j > 0; j--)
	{

	}
}
