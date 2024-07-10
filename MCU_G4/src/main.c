#include "Message.h"
#include "Middleware.h"



// Global variables
volatile uint8_t buttonPressed = 0;
volatile uint32_t timerCounter = 0;

#define ADC_MAX_VALUE 4096


//variable store current data of adc,percentage of volume
volatile uint16_t ADC_Value;
uint16_t previous_volume_value = 0;

//store string
char p[8];
//store the last send string when "WRONG"
char temp_str[8];

//index for p[10]
uint8_t i = 0;

//flag check if message is receive a full a message ( 8byte string)
uint8_t messageReceived = 0;

//status of led
//on led = true =>>>green
//on led = false =>>>red
uint8_t play = 0;



//when PC send message to MCU
//check that message
//with 3 case "W", "R", "O"
uint8_t ParseMessage(Message *msg) {
    if (messageReceived) {
		//create type field to store 2 first character
    	char first[3];

    	//create data field to store 4 next character
    	char second[5];
    	char third[3];
    	//function to store character to first and second
    	substring(p, first, 0, 2);
    	substring(p, second, 2, 4);
    	substring(p,third,6,2);
    	//function to convert string to hex then typecast to char
		char type  =  (char)StrtoHex(first,2);

		//function to convert string to hex then typecast to uint16_t
		uint16_t data = (uint16_t)StrtoHex(second,4);
		uint8_t checksumstr = (uint8_t)StrtoHex(third,2);
		//pass type,data to msg to create msg
		*msg = createMessage(type,data);



		//check if checksum is right or not
		if(msg->checksum == checksumstr){
			//reset num
			if(msg->type == 'R'){
				play =0;
			}
			//wrong, need to resent form mcu to pc
			else if(msg->type ==  'W'){
				//resent the last string send
				UART1_SendString(temp_str);
			}
			//on off led , odd : off , even :on
			else if(msg->type == 'O'){
				if(msg->data == 1){
					play =1;
				}
				else {
					play = 0;
				}
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
//Get data when PC send
void Getdata(void){
	//Receive data and save to array
	p[i] = LPUART1->DATA;
	//Get enough data (8 bytes)
	if(i >= (8 -1)){
		//Reset index
		i = 0;
		messageReceived = true;
	}
	else{
		messageReceived = false;
		i++;
	}
}

//
//void sendMessage(Message msg) {
//	char messageString[4];
//	messageToHexString(&msg, messageString);
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
	//			data : 0-4096 ~ 1- 100
	if((ADC0->SC1[0] & ADC_SC1_COCO_MASK) != 0u){
		ADC_Value = ADC_Read();
		//Calculate volume value
		uint16_t volume_value = (ADC_Read() * 100) / ADC_MAX_VALUE;
		//Parse and send message to PC
		if (volume_value >= (previous_volume_value + PERCENT_THRESHOLD)) {
			//Buffer to save string message
			char buffer[8];
			//Create message for volume
			Message a = createMessage('V',volume_value);
			//Convert message to string
			messageToHexString(&a,buffer);
			//Send message to PC
			UART1_SendString(buffer);
			//save previous string
			for(int id = 0; id < 8; id++){
				temp_str[id] = buffer[id];
			}

			//Save previous value
			previous_volume_value = volume_value;
			}
		else if (volume_value <= (previous_volume_value - PERCENT_THRESHOLD)) {
			//Buffer to save string message
			char buffer[8];
			//Create message for volume
			Message a = createMessage('V',volume_value);
			//Convert message to string
			messageToHexString(&a,buffer);
			//Send message to PC
			UART1_SendString(buffer);
			for(int id = 0; id < 8; id++){
				temp_str[id] = buffer[id];
			}
			//Save previous value
			previous_volume_value = volume_value;
			}
		}
}

void PORTC_IRQHandler(void)
{
	//SW2: Increase num
    if (PORTC->ISFR & (1 << 12))
    {
        if (buttonPressed == 0)
        {
            buttonPressed = 1;
            timerCounter = 0;
			Reset_timer_value(LPIT_TIMER_CHANNEL0,24000000);
			Start_timer(LPIT_TIMER_CHANNEL0);
            //LPIT0->TMR[0].TVAL = 24000000; // Reset timer value
            //LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK; // Enable timer
        } else
        {
            buttonPressed = 0;
            if (timerCounter < 2)
            {

				char buffer[8];
				//Create message for navigate
				Message a = createMessage('A',1);
				//Convert message to string
				messageToHexString(&a,buffer);
				//Send message to PC
				UART1_SendString(buffer);
				//Save old string
				for(int id = 0; id < 8; id++){
					temp_str[id] = buffer[id];
				}
            }
			Stop_timer(LPIT_TIMER_CHANNEL0)	;
			//LPIT0->TMR[0].TCTRL &= ~LPIT_TMR_TCTRL_T_EN_MASK; // Disable timer
        }
        PORTC->ISFR |= (1 << 12); // Clear interrupt flag
    }
    //SW3: Decrease num
    if (PORTC->ISFR & (1 << 13))
    {
        if (buttonPressed == 0)
        {
            buttonPressed = 1;
            timerCounter = 0;
			Reset_timer_value(LPIT_TIMER_CHANNEL0,24000000);
			Start_timer(LPIT_TIMER_CHANNEL0);
            // LPIT0->TMR[0].TVAL = 24000000; // Reset timer value
            // LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK; // Enable timer
        } else
        {
            buttonPressed = 0;
            if (timerCounter < 2)
            {

                char buffer[8];
                //Create message for navigate
                Message a = createMessage('A',0);
                //Convert message to string
                messageToHexString(&a,buffer);
                //Send message to PC
                UART1_SendString(buffer);
                //Save old string
                for(int id = 0; id < 8; id++){
					temp_str[id] = buffer[id];
				}
            }
			Stop_timer(LPIT_TIMER_CHANNEL0)	;
			//LPIT0->TMR[0].TCTRL &= ~LPIT_TMR_TCTRL_T_EN_MASK; // Disable timer
        }
        PORTC->ISFR |= (1 << 13); // Clear interrupt flag
    }
}

void LPIT0_Ch0_IRQHandler(void) {
    timerCounter++;
    if (timerCounter >= 2)
    {
        if (PTC->PDIR & (1 << 12))
        {	//declare buffer to store string
        	char buffer[8];
        	//Create message for select
			Message a = createMessage('S',0);
			//Convert message to string
			messageToHexString(&a,buffer);
			//Send message to PC
			UART1_SendString(buffer);
			//Save old string
			for(int id = 0; id < 8; id++){
				temp_str[id] = buffer[id];
			}
        }
    	PORTC->ISFR |= (1 << 12);
        if (PTC->PDIR & (1 << 13))
        {
        	char buffer[8];
        	//Create message for select
			Message a = createMessage('B',0);
			//Convert message to string
			messageToHexString(&a,buffer);
			//Send message to PC
			UART1_SendString(buffer);
			//Save old string
			for(int id = 0; id < 8; id++){
				temp_str[id] = buffer[id];
			}
        }
    	PORTC->ISFR |= (1 << 13);
        buttonPressed = 1;
		Stop_timer(LPIT_TIMER_CHANNEL0)	;
        //LPIT0->TMR[0].TCTRL &= ~LPIT_TMR_TCTRL_T_EN_MASK; // Disable timer
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
	LED_Init();
	SysTick_Init();
	Button_Init();
	LPIT_Timer_Init(LPIT_TIMER_CHANNEL0);
	ADC_Init(ADC0,External_Chanel12);
	while(1){
		if(messageReceived){
			receiveAndProcessMessage();
			messageReceived=0;
		}
		/*Blink LED when play or pause, stop song*/
		//
		if(play){
			LED_OFF(RED_LED);
			delay_ms(500);
			LED_Toggle(GREEN_LED);

		}
		else{
			LED_OFF(GREEN_LED);
			delay_ms(500);
			LED_Toggle(RED_LED);
		}
	}
    return 0;
}
