#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include<stdio.h>
#include"sdlib.h"
#include"emulator.h"

typedef struct{
    uint16_t opcode; 
    uint16_t NNN; // Adreça 12 bits
    uint16_t NN; // Constant 8 bits
    uint8_t N; // Constant 4 bits
    uint8_t X; // Identificador de registre, 4 bits.
    uint8_t Y; // Identificador de registre. 4 bits.
}instruction_t;


#endif
