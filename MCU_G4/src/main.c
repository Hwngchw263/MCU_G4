#include "Driver_UART.h"
#include "Driver_GPIO.h"
#include "Core_NVIC.h"
#include "Driver_ADC.h"
#include "Message.h"
#include <string.h>

extern ARM_DRIVER_GPIO Driver_GPIO;
static ARM_DRIVER_GPIO * GPIO_drv = &Driver_GPIO;
#define RED_LED					111
#define GREEN_LED               112
#define PERCENT_THRESHOLD      5

// Global variables
uint8_t buttonPressed = 0;
uint32_t timerCounter = 0;
uint16_t num = 0; //0x0000 - 0xFFFF
#define ADC_MAX_VALUE 4096


uint8_t test2 = false;
volatile uint16_t ADC_Value;
uint16_t previous_adc_percentage = 0;
char p[10];
char temp_str[10];
uint8_t i = 0;
uint8_t messageReceived = 0;
//on led = true =>>>green
//on led = false =>>>red
uint8_t on_led = 0;



void substring(const char* source, char* dest, int start, int length) {
	strncpy(dest, source + start, length);
    dest[length] = '\0';
}

void GPIO_LEDInit(void)
{
    /* Initialize pins */
	//Red led
	GPIO_drv->Setup(RED_LED,NULL);
	GPIO_drv->SetDirection(RED_LED,ARM_GPIO_OUTPUT);
	GPIO_drv->SetOutput(RED_LED,1);
	GPIO_drv->Setup(GREEN_LED,NULL);
	GPIO_drv->SetDirection(GREEN_LED,ARM_GPIO_OUTPUT);
    GPIO_drv->SetOutput(GREEN_LED,1);
}

void GPIO_Toggle(void){
	GPIO_drv->Toggle(RED_LED);
}
uint8_t ParseMessage(Message *msg) {
    if (messageReceived) {
		//create type
    	char first[3];
    	char second[5];
    	substring(p, first, 0, 2);
    	substring(p, second, 2, 4);
		//char* first = p.substr(0,2);
		////convert string to hex then typecast to char
		char type  =  (char)StrtoHex(first,2);

		//char* second = p.substr(2,4);
		//conver string to hex then typecast to uint16_t
		uint16_t data = (uint16_t)StrtoHex(second,4);

		//pass type,data to msg to create msg
		*msg = createMessage(type,data);
		uint8_t test = VerifyMessage(msg);
		if(VerifyMessage(msg)){
			//reset num
			if((msg->type == 'R')&&(msg->data == 0)){
              num = 0;
			}
			//wrong, need to resent form mcu to pc
			else if(msg->type ==  'W'){
				//resent

				UART1_SendString(temp_str);
			}
			//on off led , odd : off , even :on
			else if(msg->type == 'O'){
				on_led = ~on_led;
			}
			return 1;
		}
		else
		{
			//if wrong check sum return 0
			return 0;
		}
    } else {
    	//if not receive enough data return 0
        return 0;
    }
}
void Getdata(void){
	//pc to mcu , enable interrput
	//store that string to p[]
	//store full ->toggle
	//receive data then push into p
	p[i] = LPUART1->DATA;
	if(i >=7){
		i = 0;
		messageReceived = true;
	}
	else{
		messageReceived = false;
		i++;
	}
	num = 0;
}
//
//void sendMessage(Message msg) {
//	char messageString[4];
//	messageToString(&msg, messageString);
//    UART1_SendString(messageString);
//}
//void requestResend() {
//    Message msg = createMessage('R', 0);
//    sendMessage(msg);
//}
//

void receiveAndProcessMessage() {
    Message msg;
    if (ParseMessage(&msg)) {
        //UART1_SendString("SUCCESS");
    } else {
        //requestResend();
    	//UART1_SendString("ERROR");
}
}
void ADC0_IRQHandler(void)
{	//message : type : V ->>>0x56
	//			data : 1 ->>>0x0001 (+)
	//			checksum : have
	if((ADC0->SC1[0] & ADC_SC1_COCO_MASK) != 0u){
	    ADC_Value = ADC_Read();
	    uint16_t adc_percentage = (ADC_Read() * 100) / ADC_MAX_VALUE;

	    if (adc_percentage >= (previous_adc_percentage + PERCENT_THRESHOLD)) {
	    	//volumeup send
	        char buffer[20];
		    Message a = createMessage('V',1);
		    //save a to buffer then transform a to hexstring : 560001___
		    messageToString(&a,buffer);
		    UART1_SendString(buffer);
		    previous_adc_percentage = adc_percentage;
	    }
	    else if (adc_percentage <= (previous_adc_percentage - PERCENT_THRESHOLD)) {
	    	//volume down send
	        char buffer[20];
			Message a = createMessage('V',0);
			//save a to buffer then transform a to hexstring : 560000___
			messageToString(&a,buffer);
			UART1_SendString(buffer);
		    previous_adc_percentage = adc_percentage;
	    }

	    //previous_adc_percentage = adc_percentage;
	    //ADC0->SC1[0] |= ADC_SC1_ADCH(External_Chanel12);
	}

}

void Button_Init(void) {
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
void PORTC_IRQHandler(void)
{
	//SW2
    if (PORTC->ISFR & (1 << 12))
    {
        if (buttonPressed == 0)
        {
            buttonPressed = 1;
            timerCounter = 0;
            LPIT0->TMR[0].TVAL = 24000000; // Reset timer value
            LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK; // Enable timer
        } else
        {
            buttonPressed = 0;
            if (timerCounter < 2)
            {
                num++;
                char buffer[20];
			  //message : 41|0001|FF
			   Message a = createMessage('A',num);
			   messageToString(&a,buffer);
			   UART1_SendString(buffer);
			   //temp string
				for(int id =0;id<8;id++){
					temp_str[id] = buffer[id];
				}
            }
LPIT0->TMR[0].TCTRL &= ~LPIT_TMR_TCTRL_T_EN_MASK; // Disable timer
        }
        PORTC->ISFR |= (1 << 12); // Clear interrupt flag
    }
    //SW3
    if (PORTC->ISFR & (1 << 13))
    {
        if (buttonPressed == 0)
        {
            buttonPressed = 1;
            timerCounter = 0;
            LPIT0->TMR[0].TVAL = 24000000; // Reset timer value
            LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK; // Enable timer
        } else
        {
            buttonPressed = 0;
            if (timerCounter < 2)
            {

                num--;
                char buffer[20];
                //sprintf(buffer, "%d", num);
                //message : 41|0001|FF
                Message a = createMessage('A',num);
                messageToString(&a,buffer);
                UART1_SendString(buffer);
                //temp string
                for(int id =0;id<8;id++){
					temp_str[id] = buffer[id];
				}
            }
			LPIT0->TMR[0].TCTRL &= ~LPIT_TMR_TCTRL_T_EN_MASK; // Disable timer
        }
        PORTC->ISFR |= (1 << 13); // Clear interrupt flag
    }
}
//DONE TEMP STR
void LPIT0_Ch0_IRQHandler(void) {
    timerCounter++;
    if (timerCounter >= 2)
    {
        if (PTC->PDIR & (1 << 12))
        {
        	char buffer[20];

			//message : 41|0001|FF
			Message a = createMessage('E',0);
			messageToString(&a,buffer);
			UART1_SendString(buffer);
			//temp string
			for(int id =0;id<8;id++){
				temp_str[id] = buffer[id];
			}
        }
    	PORTC->ISFR |= (1 << 12);
        if (PTC->PDIR & (1 << 13))
        {	char buffer[20];
        	//UART1_SendChar('b');
			//message : 41|0001|FF
			Message a = createMessage('B',0);
			messageToString(&a,buffer);
			UART1_SendString(buffer);
			//temp string
			for(int id =0;id<8;id++){
				temp_str[id] = buffer[id];
			}
        }
    	PORTC->ISFR |= (1 << 13);
        buttonPressed = 1;
        LPIT0->TMR[0].TCTRL &= ~LPIT_TMR_TCTRL_T_EN_MASK; // Disable timer
    }
    LPIT0->MSR |= LPIT_MSR_TIF0_MASK; // Clear interrupt flag
}
int main(void) {
	MyUART1_Config_t cf;
	cf.baudrate = 9600;
	cf.data_length = 8;
	cf.stopbit = 1;
	cf.parity = None;
	cf.direct = LSB;
	cf.ptr = &Getdata;

	UART1_Init(&cf);
	UART1_ReceiveNonBlocking();
	GPIO_LEDInit();
	ADC_Init(ADC0,External_Chanel12);
	Button_Init();
	Timer_Init();
	while(1){
		if(messageReceived){
			receiveAndProcessMessage();
			messageReceived=0;
		}

		if(on_led){
			GPIO_drv->SetOutput(RED_LED,1);
			GPIO_drv->SetOutput(GREEN_LED,0);
		}
		else{
			GPIO_drv->SetOutput(RED_LED,0);
			GPIO_drv->SetOutput(GREEN_LED,1);
		}
	}
    return 0;
}
