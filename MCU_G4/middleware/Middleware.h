/*
 * Middleware.h
 *
 *  Created on: Jul 9, 2024
 *      Author: HUNG
 */

#ifndef MIDDLEWARE_H_
#define MIDDLEWARE_H_

#include "Driver_UART.h"
#include "Driver_GPIO.h"
#include "Core_NVIC.h"
#include "Driver_ADC.h"
#include "Core_Systick.h"
#include "Driver_LPIT.h"
extern ARM_DRIVER_GPIO Driver_GPIO;
static ARM_DRIVER_GPIO * GPIO_drv = &Driver_GPIO;
#define RED_LED					111
#define GREEN_LED               112
#define SW2  					76
#define SW3 					77
#define PERCENT_THRESHOLD      5

//Init green ,red led
//middleware function
void LED_Init(void);
void LED_Toggle(ARM_GPIO_Pin_t pin);
void LED_ON(ARM_GPIO_Pin_t pin);
void LED_OFF(ARM_GPIO_Pin_t pin);

#endif /* MIDDLEWARE_H_ */
