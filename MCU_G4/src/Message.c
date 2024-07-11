/*
 * Message.c
 *
 *  Created on: Jul 5, 2024
 *      Author: HUNG
 */
#include "Message.h"

//Function to sub string
void substring(const char* source, char* dest, int start, int length) {
	strncpy(dest, source + start, length);
    dest[length] = '\0';
}
// Function to calculate checksum
uint8_t calculateChecksum( uint16_t data) {
	// data_high
	uint8_t data_high = (data >> 8) & 0xFF;
	//data_low
	uint8_t data_low = data & 0xFF;
	uint8_t sum= data_high + data_low;
	return ~sum+1;
}
// Function to convert one byte to a hexadecimal string
void byteToHexStr(uint8_t value, char *str) {
    const char hexDigits[] = "0123456789ABCDEF";
    str[0] = hexDigits[(value >> 4) & 0x0F];
    str[1] = hexDigits[value & 0x0F];
    str[2] = '\0';
}

// Function to create message
Message createMessage(char type, uint16_t data) {
    Message msg;
    msg.type = type;
    msg.data = data;
    msg.checksum = calculateChecksum(data);
    return msg;
}

// Function to convert Message to string
void messageToHexString( Message *msg, char *str) {
	//01
    byteToHexStr((uint8_t)msg->type,&str[0]);
    //23
    byteToHexStr((uint8_t)((msg->data >> 8) & 0xFF),&str[2]);
    //45
    byteToHexStr((uint8_t)(msg->data & 0xFF),&str[4]);
    //67
    byteToHexStr((uint8_t)msg->checksum, &str[6]);
    str[8] = '\0';
}


uint8_t CharToHex(uint8_t ch_in) {
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
		c = CharToHex(ptr[idx]);
		if (16 == c)
		{
			break;
		}
		Hex_num += c * pow(16,(len - 1 - idx));
	}
    return Hex_num;
}
// Function to verify message

