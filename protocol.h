#ifndef protocol_h__
#define protocol_h__

#include <stdint.h>

#define PACKAGE_INITIATOR "a80000"
#define PROTOCOL_LENGTH 6
#define PROTOCOL_CHAR_LENGTH ((PROTOCOL_LENGTH * 2) + 1)

#define PROTOCOL_ALPHABET "57df"

void dipSwitchToProtocol(unsigned char dipSw, unsigned char bitStream[PROTOCOL_LENGTH]);
int protocolToDipSwitch(unsigned char byte[PROTOCOL_LENGTH]);
uint8_t idetifyProtocol(uint8_t trailing_begining, uint8_t trailing_end, uint8_t* bitStream, int size);

#endif