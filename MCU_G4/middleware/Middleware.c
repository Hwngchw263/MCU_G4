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
//	GPIO_drv->GPIO_Setup(SW2,NULL);
//	GPIO_drv->GPIO_Setup(SW3,NULL);
//	GPIO_drv->GPIO_SetPullResistor (SW2,ARM_GPIO_PULL_UP);
//	GPIO_drv->GPIO_SetPullResistor (SW3,ARM_GPIO_PULL_UP);
//
//	GPIO_drv->SetEventTrigger(SW2,ARM_GPIO_TRIGGER_EITHER_EDGE);
//	GPIO_drv->SetEventTrigger(SW3,ARM_GPIO_TRIGGER_EITHER_EDGE);
//	GPIO_drv->SetDirection(SW2,ARM_GPIO_INPUT);
//	GPIO_drv->SetDirection(SW3,ARM_GPIO_INPUT);
    PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;

    PORTC->PCR[12] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_IRQC(0xB);
    PORTC->PCR[13] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_IRQC(0xB);
    PTC->PDDR &= ~(1 << 12);
    PTC->PDDR &= ~(1 << 13);
    // Enable Interrupts for PORTC
    NVIC_EnableIRQ(PORTC_IRQn);
}

void Timer_Init(void) {
    //
 	PCC->PCCn[PCC_LPIT_INDEX] &= ~PCC_PCCn_CGC(1u);
	SCG->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;
	SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1);
	SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN(1);
	PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_PCS(0b011);
	PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC(1u);
	LPIT0->MCR |= (1<<LPIT_MCR_M_CEN_SHIFT) | (1<<LPIT_MCR_DBG_EN_SHIFT);
    LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_MODE(0u);
    LPIT0->TMR[0].TVAL = 48000000; // Set timer value for 2 seconds (assuming 2MHz clock)
   // LPIT0->TMR[0].TCTRL = LPIT_TMR_TCTRL_TIE_MASK; // Enable interrupt
    LPIT0->MIER |= (1 << LPIT_MIER_TIE0_SHIFT);
    // Enable Interrupts for LPIT0s
    NVIC_EnableIRQ(LPIT0_Ch0_IRQn);
}
