/*
 * Message.h
 *
 *  Created on: Jul 5, 2024
 *      Author: HUNG
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_
#include "stdbool.h"
#include<string.h>
#include <stdio.h>
#define MESSAGE_LENGTH  8
#define Hex_Error		16
typedef struct {
    char type;
    uint16_t data;
    uint8_t checksum;
} Message;

//Function to sub string
void substring(const char* source, char* dest, int start, int length);
// Function to calculate checksum
uint8_t calculateChecksum( uint16_t data);
// Function to create message
Message createMessage(char type, uint16_t data);
// Function to convert Message to string
void messageToHexString(Message *msg, char *str);


#endif /* MESSAGE_H_ */
