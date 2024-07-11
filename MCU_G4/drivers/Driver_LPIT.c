#include "Driver_LPIT.h"
#include "Core_NVIC.h"

void LPIT_Timer_Init(Lpit_timer_t channel){
	PCC->PCCn[PCC_LPIT_INDEX] &= ~PCC_PCCn_CGC(1u);
	SCG->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;
	SCG->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(1);
	SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN(1);
	PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_PCS(0b011);
	PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC(1u);
	LPIT0->MCR |= (1<<LPIT_MCR_M_CEN_SHIFT) | (1<<LPIT_MCR_DBG_EN_SHIFT);
	LPIT0->TMR[channel].TCTRL |= LPIT_TMR_TCTRL_MODE(0u);
	LPIT0->TMR[channel].TVAL = 48000000; // Set timer value for 2 seconds (assuming 2MHz clock)
   // LPIT0->TMR[0].TCTRL = LPIT_TMR_TCTRL_TIE_MASK; // Enable interrupt
	switch(channel){
	case LPIT_TIMER_CHANNEL0:
		LPIT0->MIER |= (1 << LPIT_MIER_TIE0_SHIFT);
		NVIC_EnableIRQ(LPIT0_Ch0_IRQn);
		break;
	case LPIT_TIMER_CHANNEL1:
		LPIT0->MIER |= (1 << LPIT_MIER_TIE1_SHIFT);
		NVIC_EnableIRQ(LPIT0_Ch1_IRQn);
		break;
	case LPIT_TIMER_CHANNEL2:
		LPIT0->MIER |= (1 << LPIT_MIER_TIE2_SHIFT);
		NVIC_EnableIRQ(LPIT0_Ch2_IRQn);
		break;
	case LPIT_TIMER_CHANNEL3:
		LPIT0->MIER |= (1 << LPIT_MIER_TIE3_SHIFT);
		NVIC_EnableIRQ(LPIT0_Ch3_IRQn);
		break;
	}


//	LPIT0->MIER |= (1 << LPIT_MIER_TIE0_SHIFT);
//	// Enable Interrupts for LPIT0s
//	NVIC_EnableIRQ(LPIT0_Ch0_IRQn);
}

void Reset_timer_value(Lpit_timer_t channel,uint32_t value){
	LPIT0->TMR[channel].TVAL = value;
}
void Start_timer(Lpit_timer_t channel){
	LPIT0->TMR[channel].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;
}
void Stop_timer(Lpit_timer_t channel){
	LPIT0->TMR[channel].TCTRL &= ~LPIT_TMR_TCTRL_T_EN_MASK;
}
