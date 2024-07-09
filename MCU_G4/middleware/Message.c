/*
 * Message.c
 *
 *  Created on: Jul 5, 2024
 *      Author: HUNG
 */
#include "Message.h"

uint8_t calculateChecksum( uint16_t data) {
	uint8_t data_high = (data >> 8) & 0xFF;
	//data_low =1100
	uint8_t data_low = data & 0xFF;
	uint8_t sum= data_high + data_low;
	return ~sum +1;
}
void uint8toHexString(uint8_t value, char *str) {
    const char hexDigits[] = "0123456789ABCDEF";
    str[0] = hexDigits[(value >> 4) & 0x0F];
    str[1] = hexDigits[value & 0x0F];
    str[2] = '\0';
}
void uint16toHexString(uint16_t value , char *str){
	const char hexDigits[] = "0123456789ABCDEF";
	str[0] = hexDigits[(value >> 12) & 0x0F];
	str[1] = hexDigits[(value >> 8) & 0x0F];
	str[2] = hexDigits[(value >> 4) & 0x0F];
	str[3] = hexDigits[value & 0x0F];
	str[4] = '\0'; // Null-terminate the string
}

Message createMessage(char type, uint16_t data) {
    Message msg;
    msg.type = type;
    msg.data = data;
    msg.checksum = calculateChecksum(data);
    return msg;
}
void messageToString( Message *msg, char *str) {
	//01
    uint8toHexString((uint8_t)msg->type,&str[0]);
    //2345
    uint16toHexString(msg->data, &str[2]);
    //67
    uint8toHexString((uint8_t)msg->checksum, &str[6]);
    str[8] = '\0';
}


uint8_t Convert_Char_To_Hex(uint8_t ch_in) {
	uint8_t hex_out = Hex_Error;
	if (('0' <= ch_in) && (ch_in <= '9'))

	{
		hex_out = ch_in - '0';
	}
	else if (('A' <= ch_in) && (ch_in <= 'F'))
	{
		hex_out = ch_in - 'A' + 0xA;
	}
	return hex_out;
}
uint32_t StrtoHex(char* ptr, uint8_t len)
{
    uint32_t  Hex_num = 0;
	uint8_t idx,c;
    //output result is Hex_num
	for (idx = 0; idx < len; idx++)
	{
		c = Convert_Char_To_Hex(ptr[idx]);
		if (16 == c)
		{
			break;
		}
		Hex_num += c * pow(16,(len - 1 - idx));
	}
    return Hex_num;
}
bool VerifyMessage(Message* msg){
    uint8_t check = msg->checksum;
    if(check == calculateChecksum(msg->data)){
        return true;
    }
    return false;
}
