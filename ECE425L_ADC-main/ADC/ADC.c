/**
 * @file ADC.c
 *
 * @brief Source code for the ADC driver.
 *
 * This file contains the function definitions for the ADC driver.
 *
 * ADC Module 0 is used to sample the potentiometer and the analog
 * light sensor that are connected on the EduBase board. ADC Module 0
 * is configured to sample the potentiometer first and the light sensor after.
 *
 * After the light sensor is sampled, an interrupt signal is set to 
 * indicate that the sampling sequence has ended. After the conversion
 * results have been read from the corresponding FIFO, the interrupt is cleared.
 *
 * The following pins are used:
 *  - Potentiometer   <-->  Tiva LaunchPad Pin PE2 (Channel 1)
 *  - Light Sensor    <-->  Tiva LaunchPad Pin PE1 (Channel 2)
 *
 * @author
 *
 */

#include "ADC.h"

void ADC_Init(void)
{
	//Enable the clock to ADC Module 0 by setting the R0 bit (Bit 0) in the RCGCADC
	//register and provide a short delay of 1 ms after enabling the clock
	SYSCTL->RCGCADC |= 0x01;
	SysTick_Delay1ms(1);
	
	//Enable the clock to Port E by setting the R4 bit (Bit 4) in the RCGCGPIO register
	SYSCTL->RCGCGPIO |= 0x10;
	
	//Configure PE2 pin
	GPIOE->DIR &= ~0x04;
	
	//Disable the digitial functionality for the PE2 pin by clearing Bit2
	//in the DEN register
	GPIOE->DEN &= ~0x04;
	
	//Enable the analog functionality for the PE2 pin by setting Bit 2 in the AMSEL
	GPIOE->AMSEL |= 0x04;
	
	//Select the alternative function for the PE2 pin by setting Bit 2
	GPIOE->AFSEL |= 0x04;
	
	GPIOE->DIR &= ~0x02;
	GPIOE->DEN &= ~0x02;
	GPIOE->AMSEL |= 0x02;
	GPIOE->AFSEL |= 0x02;
	
	//Before configuration, disable the Sample Sequencer 0 module by clearing the ASENO
	ADC0->ACTSS &= ~0x1;
	
	//Clearing all bits
	//Enables the Sample Sequencer 0 module to sample
	ADC0->EMUX &= ~0x000F;
	
	//Clear all the bits(Bits 31 to 0) in the ADC Sample Sequence Input Multiplexer
	ADC0->EMUX &= ~0xFFFFFFFF;
	
	//Choose Channel 1(PE2, potentiometer)
	//Changed Task 1
	//Sample 0: Channel 1(PE2)
	//Sample 1: Channel 2(PE1)
	ADC0->SSMUX0 |= 0x00000021;
	
	//Assign Channel 1 to be the end of the sampling sequence by setting the END0 bit
	//Deleted Task 1
	//ADC0->SSCTL0 |= 0x00000002;
	
	//Enable the raw interrupt signal for the first sample by setting the IE0 bit
	//Changed Task 1
	//End sample sequence from table 13.1 it ends at 6
	ADC0->SSCTL0 |= 0x00000006;
	
	//Enable the Sample Sequencer 0 module by setting the ASEN0 bit(Bit 0)
	ADC0->ACTSS |= 0x1;

}

void ADC_Sample(double analog_value_buffer[])
{
	ADC0->PSSI |= 0x01;
	
	while((ADC0->RIS & 0x01) == 0);
	
	uint32_t potentiometer_sample_result = ADC0->SSFIFO0;
	uint32_t light_sensor_sample_result = ADC0->SSFIFO0;
	
	ADC0->ISC |= 0x01;
	
	analog_value_buffer[0] = (potentiometer_sample_result * 3.3) / 4096;
	analog_value_buffer[0] = (light_sensor_sample_result * 3.3) / 4096;
}

int Analog_Voltage_to_Digital(double voltage, double in_min, double in_max, int out_min, int out_max)
{
    if (voltage <= in_min) 
		{
			return out_min;
		}
    if (voltage >= in_max) 
		{
			return out_max;
		}

    return (int)((voltage - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}