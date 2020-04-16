/**
	\file GPIO.c
	\brief
		This is the source file for the GPIO device driver for Kinetis K64.
		It contains all the implementation for configuration functions and runtime functions.
		i.e., this is the application programming interface (API) for the GPIO peripheral.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	18/02/2019
	\TODO: callbacks are not implemented on this api
	    Interrupts are not implemented in this API implementation.
 */

#include "MK64F12.h"
#include "GPIO.h"
#include "bits.h"


static GPIO_interrupt_flags_t g_gpio_inter_status = {0};
static void (*GPIOn_callback[5])(void);



/*GPIO callback initialization*/
void GPIO_callback_init(gpio_port_name_t portName, void(*handler)(void))
{
	GPIOn_callback[portName] = handler;
}


/**Interruption vectors detected*/
void PORTA_IRQHandler(void)
{
	g_gpio_inter_status.GPIOA_flag_status =  TRUE;
	GPIO_clear_interrupt(GPIO_A);

	if(GPIOn_callback[GPIO_A])
	{
		GPIOn_callback[GPIO_A]();
	}
}

void PORTB_IRQHandler(void)
{
	g_gpio_inter_status.GPIOB_flag_status =  TRUE;
	GPIO_clear_interrupt(GPIO_B);

	if(GPIOn_callback[GPIO_B])
	{
		GPIOn_callback[GPIO_B]();
	}

}

void PORTC_IRQHandler(void)
{
	g_gpio_inter_status.GPIOC_flag_status =  TRUE;
	GPIO_clear_interrupt(GPIO_C);

	if(GPIOn_callback[GPIO_C])
	{
		GPIOn_callback[GPIO_C]();
	}

}

void PORTD_IRQHandler(void)
{
	g_gpio_inter_status.GPIOD_flag_status =  TRUE;
	GPIO_clear_interrupt(GPIO_D);

	if(GPIOn_callback[GPIO_D])
	{
		GPIOn_callback[GPIO_D]();
	}

}

void PORTE_IRQHandler(void)
{
	g_gpio_inter_status.GPIOE_flag_status =  TRUE;
	GPIO_clear_interrupt(GPIO_E);

	if(GPIOn_callback[GPIO_E])
	{
		GPIOn_callback[GPIO_E]();
	}

}

/*Clear interrupt flag status selected */
uint8_t GPIO_clear_irq_status(gpio_port_name_t port_name)
{
	switch(port_name)/** Selecting the GPIO to  clear flag*/
		{
		case GPIO_A: /*Clear programmer flag status*/
			g_gpio_inter_status.GPIOA_flag_status &= OFF_VAL;
			break;
		case GPIO_B: /*Clear programmer flag status*/
			g_gpio_inter_status.GPIOB_flag_status &= OFF_VAL;
			break;
		case GPIO_C: /*Clear programmer flag status*/
			g_gpio_inter_status.GPIOC_flag_status &= OFF_VAL;
			break;
		case GPIO_D: /*Clear programmer flag status*/
			g_gpio_inter_status.GPIOD_flag_status &= OFF_VAL;
			break;
		case GPIO_E: /*Clear programmer flag status*/
			g_gpio_inter_status.GPIOE_flag_status &= OFF_VAL;
			break;
		default: /*If any port was selected*/
			return FALSE;
			break;

		}// end switch
	return TRUE;
}

/*Clear interrupt flag status selected */
uint8_t GPIO_get_irq_status(gpio_port_name_t port_name)
{
	uint8_t GPIOn_flag;
	switch(port_name)/* Selecting the GPIO to  return flag*/
		{
		case GPIO_A: /*Get programmer flag status*/
			GPIOn_flag = g_gpio_inter_status.GPIOA_flag_status;
			break;
		case GPIO_B: /*Get programmer flag status*/
			GPIOn_flag = g_gpio_inter_status.GPIOB_flag_status;
			break;
		case GPIO_C: /*Get programmer flag status*/
			GPIOn_flag = g_gpio_inter_status.GPIOC_flag_status;
			break;
		case GPIO_D: /*Get programmer flag status*/
			GPIOn_flag = g_gpio_inter_status.GPIOD_flag_status;
			break;
		case GPIO_E: /*Get programmer flag status*/
			GPIOn_flag = g_gpio_inter_status.GPIOE_flag_status;
			break;
		default: /*If any port was selected*/
			return FALSE;
			break;

		}// end switch
	return GPIOn_flag;
}

/**Clear interrupts once they were used*/
void GPIO_clear_interrupt(gpio_port_name_t port_name)
{
	switch(port_name)/** Selecting the GPIO for clock enabling*/
	{
	case GPIO_A: /** GPIO A is selected*/
		PORTA->ISFR = 0xFFFFFFFF;
		break;
	case GPIO_B: /** GPIO B is selected*/
		PORTB->ISFR = 0xFFFFFFFF;
		break;
	case GPIO_C: /** GPIO C is selected*/
		PORTC->ISFR = 0xFFFFFFFF;
		break;
	case GPIO_D: /** GPIO D is selected*/
		PORTD->ISFR = 0xFFFFFFFF;
		break;
	default: /** GPIO E is selected*/
		PORTE->ISFR = 0xFFFFFFFF;
		break;

	}// end switch
}

uint8_t init_bit_as_falling_int_input(gpio_port_name_t portName, bit_t bit)
{
	gpio_pin_control_register_t pinControlRegister =  GPIO_MUX1|GPIO_PS|GPIO_PE|INTR_FALLING_EDGE;
	/*Pin Control Configuration as GPIO Mux1 and pull-up resistor selected and enabled*/
	GPIO_clock_gating(portName);
	GPIO_pin_control_register(portName, bit,  &pinControlRegister);
	GPIO_data_direction_pin(portName,	GPIO_INPUT , bit);
	return TRUE;

}
uint8_t init_bit_as_output(gpio_port_name_t portName, bit_t bit)
{
	gpio_pin_control_register_t mux_value = GPIO_MUX1;
	/*Pin Control Configuration as GPIO Mux1 and pull-up resistor selected and enabled*/
	GPIO_clock_gating(portName);
	GPIO_pin_control_register(portName, bit,  &mux_value);
	GPIO_data_direction_pin(portName,	GPIO_OUTPUT, bit);
	return TRUE;
}


uint8_t GPIO_switch_init(gpio_as_switch SW_n)
{
	gpio_pin_control_register_t pinControlRegisterGPIOCpin6 = GPIO_MUX1|GPIO_PS|GPIO_PE|INTR_FALLING_EDGE;
	gpio_pin_control_register_t pinControlRegisterGPIOApin4 = GPIO_MUX1|GPIO_PS|GPIO_PE|INTR_FALLING_EDGE;
	switch(SW_n)
	{
	case SW_2:
		/*Pin Control Configuration as GPIO Mux1 and pull-up resistor selected and enabled*/
		GPIO_clock_gating(GPIO_C);
		GPIO_pin_control_register(GPIO_C,bit_6,  &pinControlRegisterGPIOCpin6);
		GPIO_data_direction_pin(GPIO_C,	GPIO_INPUT, bit_6);

		break;
	case SW_3:
		/*Pin Control Configuration as GPIO Mux1 and pull-up resistor selected and enabled*/

		GPIO_clock_gating(GPIO_A);
		GPIO_pin_control_register(GPIO_A,bit_4,  &pinControlRegisterGPIOApin4);
		GPIO_data_direction_pin(GPIO_A,	GPIO_INPUT,	bit_4);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

uint8_t GPIO_clock_gating(gpio_port_name_t port_name)
{
	switch(port_name)/** Selecting the GPIO for clock enabling*/
			{
				case GPIO_A: /** GPIO A is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTA; /** Bit 9 of SIM_SCGC5 is  set*/
					break;
				case GPIO_B: /** GPIO B is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTB; /** Bit 10 of SIM_SCGC5 is set*/
					break;
				case GPIO_C: /** GPIO C is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTC; /** Bit 11 of SIM_SCGC5 is set*/
					break;
				case GPIO_D: /** GPIO D is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTD; /** Bit 12 of SIM_SCGC5 is set*/
					break;
				case GPIO_E: /** GPIO E is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTE; /** Bit 13 of SIM_SCGC5 is set*/
					break;
				default: /**If doesn't exist the option*/
					return(FALSE);
			}// end switch
	/**Successful configuration*/
	return(TRUE);
}// end function

uint8_t GPIO_pin_control_register(gpio_port_name_t port_name, uint8_t pin,const gpio_pin_control_register_t*  pin_control_register)
{

	switch(port_name)
		{
		case GPIO_A:/** GPIO A is selected*/
			PORTA->PCR[pin] = *pin_control_register;
			break;
		case GPIO_B:/** GPIO B is selected*/
			PORTB->PCR[pin] = *pin_control_register;
			break;
		case GPIO_C:/** GPIO C is selected*/
			PORTC->PCR[pin] = *pin_control_register;
			break;
		case GPIO_D:/** GPIO D is selected*/
			PORTD->PCR[pin] = *pin_control_register;
			break;
		case GPIO_E: /** GPIO E is selected*/
			PORTE->PCR[pin]= *pin_control_register;
		default:/**If doesn't exist the option*/
			return(FALSE);
		break;
		}
	/**Successful configuration*/
	return(TRUE);
}

void GPIO_write_port(gpio_port_name_t port_name, uint32_t data)
{
	switch(port_name)
	{
	case GPIO_A: /**GPIO A will be written*/
		GPIOA->PDOR = data;
		break;
	case GPIO_B: /**GPIO B will be written*/
		GPIOB->PDOR = data;
		break;
	case GPIO_C: /**GPIO C will be written*/
		GPIOC->PDOR = data;
		break;
	case GPIO_D: /**GPIO D will be written*/
		GPIOD->PDOR = data;
		break;
	case GPIO_E: /**GPIO E will be written*/
		GPIOE->PDOR = data;
		break;
	}
}

void GPIO_data_direction_pin(gpio_port_name_t port_name, uint8_t state, uint8_t pin)
{
	switch(port_name)
	{
	case GPIO_A: /**GPIOA will be configured*/
		GPIOA->PDDR &= ~(TRUE << pin);
		GPIOA->PDDR |= (state<<pin);
		break;
	case GPIO_B: /**GPIOB will be configured*/
		GPIOB->PDDR &= ~(TRUE << pin);
		GPIOB->PDDR |= (state<<pin);
		break;
	case GPIO_C: /**GPIOC will be configured*/
		GPIOC->PDDR &= ~(TRUE << pin);
		GPIOC->PDDR |= (state<<pin);
		break;
	case GPIO_D: /**GPIOD will be configured*/
		GPIOD->PDDR &= ~(TRUE << pin);
		GPIOD->PDDR |= (state<<pin);
		break;
	case GPIO_E: /**GPIOE will be configured*/
		GPIOE->PDDR &= ~(TRUE << pin);
		GPIOE->PDDR |= (state<<pin);
		break;
	}

}


uint8_t GPIO_read_pin(gpio_port_name_t portName, uint8_t pin)
{
	uint32_t input_value = TRUE;
	switch(portName)
			{
			case GPIO_A: /**GPIO A will be written*/
				input_value = GPIOA->PDIR & (TRUE << pin);
				break;
			case GPIO_B: /**GPIO B will be written*/
				input_value = GPIOB->PDIR & (TRUE << pin);
				break;
			case GPIO_C: /**GPIO C will be written*/
				input_value = GPIOC->PDIR & (TRUE << pin);
				break;
			case GPIO_D: /**GPIO D will be written*/
				input_value = GPIOD->PDIR & (TRUE << pin);
				break;
			case GPIO_E: /**GPIO E will be written*/
				input_value = GPIOE->PDIR & (TRUE << pin);
				break;
			default:
				input_value = TRUE;
			}
	return input_value;
}
void GPIO_set_pin(gpio_port_name_t portName, uint8_t pin)
{
	switch(portName)
		{
		case GPIO_A: /**GPIO A will be written*/
			GPIOA->PSOR = (TRUE << pin);
			break;
		case GPIO_B: /**GPIO B will be written*/
			GPIOB->PSOR = (TRUE << pin);
			break;
		case GPIO_C: /**GPIO C will be written*/
			GPIOC->PSOR = (TRUE << pin);
			break;
		case GPIO_D: /**GPIO D will be written*/
			GPIOD->PSOR = (TRUE << pin);
			break;
		case GPIO_E: /**GPIO E will be written*/
			GPIOE->PSOR = (TRUE << pin);
			break;
		}

}

void GPIO_clear_pin(gpio_port_name_t portName, uint8_t pin)
{
	switch(portName)
			{
			case GPIO_A: /**GPIO A will be written*/
				GPIOA->PCOR = (TRUE << pin);
				break;
			case GPIO_B: /**GPIO B will be written*/
				GPIOB->PCOR = (TRUE << pin);
				break;
			case GPIO_C: /**GPIO C will be written*/
				GPIOC->PCOR = (TRUE << pin);
				break;
			case GPIO_D: /**GPIO D will be written*/
				GPIOD->PCOR = (TRUE << pin);
				break;
			case GPIO_E: /**GPIO E will be written*/
				GPIOE->PCOR = (TRUE << pin);
				break;
			}
}
void GPIO_toogle_pin(gpio_port_name_t portName, uint8_t pin)
{
	switch(portName)
			{
			case GPIO_A: /**GPIO A will be written*/
				GPIOA->PTOR = (TRUE << pin);
				break;
			case GPIO_B: /**GPIO B will be written*/
				GPIOB->PTOR = (TRUE << pin);
				break;
			case GPIO_C: /**GPIO C will be written*/
				GPIOC->PTOR = (TRUE << pin);
				break;
			case GPIO_D: /**GPIO D will be written*/
				GPIOD->PTOR = (TRUE << pin);
				break;
			case GPIO_E: /**GPIO E will be written*/
				GPIOE->PTOR = (TRUE << pin);
				break;
			}
}

uint32_t GPIO_read_port(gpio_port_name_t portName)
{
	uint32_t input_value = TRUE;
		switch(portName)
				{
				case GPIO_A: /**GPIO A will be written*/
					input_value = GPIOA->PDIR;
					break;
				case GPIO_B: /**GPIO B will be written*/
					input_value = GPIOB->PDIR;
					break;
				case GPIO_C: /**GPIO C will be written*/
					input_value = GPIOC->PDIR;
					break;
				case GPIO_D: /**GPIO D will be written*/
					input_value = GPIOD->PDIR;
					break;
				case GPIO_E: /**GPIO E will be written*/
					input_value = GPIOE->PDIR;
					break;
				default:
					input_value = TRUE;
				}
		return input_value;
}
void GPIO_data_direction_port(gpio_port_name_t portName ,uint32_t direction)
{
	switch(portName)
		{
		case GPIO_A: /**GPIOA will be configured*/
			GPIOA->PDDR = direction;
			break;
		case GPIO_B: /**GPIOB will be configured*/
			GPIOB->PDDR = direction;
			break;
		case GPIO_C: /**GPIOC will be configured*/
			GPIOC->PDDR = direction;
			break;
		case GPIO_D: /**GPIOD will be configured*/
			GPIOD->PDDR = direction;
			break;
		case GPIO_E: /**GPIOE will be configured*/
			GPIOE->PDDR= direction;
			break;
		}
}
