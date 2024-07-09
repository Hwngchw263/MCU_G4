/*
 * Middleware.c
 *
 *  Created on: Jul 9, 2024
 *      Author: HUNG
 */


//Init green ,red led
//middleware function
#include "Middleware.h"
void LED_Init(void)
{
    /* Initialize pins */
	//Red led
	GPIO_drv->Setup(RED_LED,NULL);
	GPIO_drv->SetDirection(RED_LED,ARM_GPIO_OUTPUT);
	GPIO_drv->SetOutput(RED_LED,1);
	//Green led
	GPIO_drv->Setup(GREEN_LED,NULL);
	GPIO_drv->SetDirection(GREEN_LED,ARM_GPIO_OUTPUT);
    GPIO_drv->SetOutput(GREEN_LED,1);
}

//function to toggle red ( not use)
void LED_Toggle(ARM_GPIO_Pin_t pin){
	GPIO_drv->Toggle(pin);
}
void LED_ON(ARM_GPIO_Pin_t pin){
	GPIO_drv->SetOutput(pin,0);
}
void LED_OFF(ARM_GPIO_Pin_t pin){
	GPIO_drv->SetOutput(pin,1);
}

void Button_Init(void) {
	GPIO_drv->Setup(SW2,NULL);
	GPIO_drv->Setup(SW3,NULL);
	GPIO_drv->SetPullResistor (SW2,ARM_GPIO_PULL_UP);
	GPIO_drv->SetPullResistor (SW3,ARM_GPIO_PULL_UP);

	GPIO_drv->SetEventTrigger(SW2,ARM_GPIO_TRIGGER_EITHER_EDGE);
	GPIO_drv->SetEventTrigger(SW3,ARM_GPIO_TRIGGER_EITHER_EDGE);
	GPIO_drv->SetDirection(SW2,ARM_GPIO_INPUT);
	GPIO_drv->SetDirection(SW3,ARM_GPIO_INPUT);
//    PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
//
//    PORTC->PCR[12] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_IRQC(0xB);
//    PORTC->PCR[13] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_IRQC(0xB);
//    PTC->PDDR &= ~(1 << 12);
//    PTC->PDDR &= ~(1 << 13);
    // Enable Interrupts for PORTC
    NVIC_EnableIRQ(PORTC_IRQn);
}


