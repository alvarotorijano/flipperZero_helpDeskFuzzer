#include "protocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void strrev(char* str) {
    int i = 0, j = strlen(str) - 1;
    while(i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

void decimal_to_base4(unsigned char decimal, char base4[]) {
    strcpy(base4, "0000");
    int i = 0;
    while(decimal > 0) {
        int remainder = decimal % 4;
        decimal = decimal / 4;
        base4[i++] = remainder + '0';
    }
    strrev(base4);
    base4[5] = '\0';
}

int base4_to_decimal(char* base4) {
    int decimal = 0;
    int len = strlen(base4);
    for(int i = 0; i < len; i++) {
        decimal = decimal * 4 + (base4[i] - '0');
    }
    return decimal;
}

void translate(char* input, char* input_alphabet, char* output_alphabet) {
    int input_len = strlen(input);
    int alphabet_len = strlen(input_alphabet);
    for(int i = 0; i < input_len; i++) {
        for(int j = 0; j < alphabet_len; j++) {
            if(input[i] == input_alphabet[j]) {
                input[i] = output_alphabet[j];
                break;
            }
        }
    }
}

void encode(char input[], char output[], char alphabet[], int alphabetSize) {
    char string[2] = {0, 0};
    for(int i = 0; i < strlen(input); i++) {
        string[0] = input[i];
        printf("\n%d", atoi(string));
        if(atoi(string) < alphabetSize) {
            output[i] = alphabet[atoi(string)];

        } else {
            output[i] = input[0];
        }
    }
    output[5] = '\0';
}

unsigned char* hex_to_bytes(char hex_string[]) {
    int len = strlen(hex_string);
    unsigned char* bytes = malloc(len / 2);

    for(int i = 0; i < len; i += 2) {
        sscanf(&hex_string[i], "%2hhx", &bytes[i / 2]);
    }

    return bytes;
}

void dipSwitchToProtocol(unsigned char dipSw, unsigned char bitStream[PROTOCOL_LENGTH]) {
    char original[5] = "0000";
    char output_protocol[PROTOCOL_CHAR_LENGTH];
    memset(output_protocol, 0, PROTOCOL_CHAR_LENGTH);
    decimal_to_base4(dipSw / 2, original);
    translate(original, "0123", PROTOCOL_ALPHABET);
    strcat(output_protocol, PACKAGE_INITIATOR);
    strcat(output_protocol, original);
    if(dipSw % 2) {
        strcat(output_protocol, "80");
    } else {
        strcat(output_protocol, "00");
    }
    memcpy(bitStream, hex_to_bytes(output_protocol), PROTOCOL_LENGTH);
}

int protocolToDipSwitch(unsigned char byte[PROTOCOL_LENGTH]) {
    int dipSwitchStatus = 0;
    char relevantBytes[5];
    sprintf(relevantBytes, "%.2x%.2x", byte[3], byte[4]);
    translate(relevantBytes, PROTOCOL_ALPHABET, "0123");
    dipSwitchStatus = base4_to_decimal(relevantBytes) * 2;

    if(byte[5]) {
        dipSwitchStatus += 1;
    }
    return dipSwitchStatus;
}

uint8_t isContained(char letter, char* str) {
    for(int i = 0; str[i] != 0; i++) {
        if(letter == str[i]) {
            return 1;
        }
    }
    return 0;
}

uint8_t checkByte(uint8_t byte, char* alphabet) {
    char hex[3];
    sprintf(hex, "%02x", byte); // convierte el byte a hexadecimal
    int i;
    for(i = 0; i < 2; i++) {
        if(isContained(hex[i], alphabet) == 0) { // busca cada caracter en el alfabeto
            return 0;
        }
    }
    return 1;
}

uint8_t checkPattern(uint8_t trailing_begin, uint8_t trailing_end, uint8_t* byte) {
    int i = 0;
    for(int j = 0; j < trailing_begin; j++) {
        if(byte[i] != 0) {
            return 0;
        }
        i++;
    }
    if((byte[i] != 0xa8) || (byte[i + 1] != 0x00) || (byte[i + 2] != 0x00) ||
       !checkByte(byte[i + 3], PROTOCOL_ALPHABET) || !checkByte(byte[i + 4], PROTOCOL_ALPHABET) ||
       (byte[i + 5] != 0x00 && byte[i + 5] != 0x80)) {
        return 0;
    }
    i += PROTOCOL_LENGTH;

    for(int j = 0; j < trailing_end; j++, i++) {
        if(byte[i] != 0) {
            return 0;
        }
    }
    return 1;
}

uint8_t idetifyProtocol(uint8_t trailing_begining, uint8_t trailing_end, uint8_t* bitStream, int size) {
    for(int i = 0; i < size - (trailing_begining + PROTOCOL_LENGTH + trailing_end); i++) {
        if(checkPattern(trailing_begining, trailing_end, bitStream + i)) {
            return 1;
        }
    }
    return 0;
}
