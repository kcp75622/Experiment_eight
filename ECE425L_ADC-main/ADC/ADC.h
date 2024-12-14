/**
 * @file ADC.h
 *
 * @brief Header file for the ADC driver.
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

#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"

/**
 * @brief
 *
 * @param
 *
 * @return
 */
void ADC_Init(void);

/**
 * @brief
 *
 * @param
 *
 * @return
*/
void ADC_Sample(double analog_value_buffer[]);

int Analog_Voltage_to_Digital(double voltage, double in_min, double in_max, int out_min, int out_max);
