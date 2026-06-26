#ifndef EMULATOR_H
#define  EMULATOR_H
#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include"sdlib.h"

typedef struct{
    uint16_t opcode;
    uint8_t cat; //Bit de major significancia. 
    uint16_t NNN; // Adreça 12 bits
    uint16_t NN; // Constant 8 bits
    uint8_t N; // Constant 4 bits
    uint8_t X; // Identificador de registre, 4 bits.
    uint8_t Y; // Identificador de registre. 4 bits.
}instruction_t;




typedef enum {
    QUIT,
    RUNNING,
    PAUSED,
}em_state;

typedef struct{

    em_state state;
    uint8_t ram[4096];
    uint8_t display[64*32]; // Pantalla. Traduir de bool display[64*32] o bé uint8_t display[64*32] 
    uint8_t v[16]; // Registres.
    uint16_t stack[12];
    uint16_t *sp;
    uint16_t I; // Registre d'adreça 12 bits.
    uint8_t delay_timer; //Redueix valor cada 60 hz
    uint8_t sound_timer; //Redueix valor cada 60hz i fa to.
    uint16_t keypad; // 16 bits per a les tecles.
    char *rom_name; //El ROM a correr.
    uint16_t pc;  // PROGRAM COUNTER!!
    instruction_t actual; //instruccio actual.
}chip8_t;

int init_chip8(chip8_t *chip8, const char rom_name[]);

void handle_input(chip8_t * chip8);

void emulate_instruction(chip8_t* chip8);

// INSTRUCCIONS D'OPCODES:

void inst_neteja_pantalla(chip8_t *chip8); // Instrucció: 00E0
                                         
void inst_retorn_subrutina(chip8_t *chip8); //Instrucció: 00EE
                                           
void inst_salt(chip8_t *chip8); // Instrucció: 1NNN
                                
void inst_entrar_subrutina(chip8_t *chip8); // Instrucció: 2NNN
                                            
void inst_next_igual(chip8_t* chip8); // Instrucció: 3XNN 

void inst_next_diferent(chip8_t* chip8);// Instrucció: 4XNN
                                        
void inst_next_igual_registres(chip8_t* chip8);// Instrucció: 5XY0
                                               
void inst_inicialitzar_registre_immediat(chip8_t* chip8);// Instrucció: 6XNN
                                                         
void inst_suma_immediat(chip8_t* chip8);// Instrucció: 7XNN
                                        
void inst_igualar_registre(chip8_t* chip8);// Instrucció: 8XY0
void inst_or_registre(chip8_t* chip8);// Instrucció: 8XY1
void inst_and_registre(chip8_t* chip8);// Instrucció: 8XY2
void inst_xor_registre(chip8_t* chip8);// Instrucció: 8XY3
void inst_suma_registre(chip8_t* chip8);// Instrucció: 8XY4
void inst_resta_registre(chip8_t* chip8);// Instrucció: 8XY5
void inst_dividir_entredos(chip8_t* chip8);// Instrucció: 8XY6
void inst_resta_inversa(chip8_t* chip8);// Instrucció: 8XY7
void inst_multiplica_perdos(chip8_t* chip8);// Instrucció: 8XYE
                                            
void inst_comparar_registres(chip8_t* chip8);// Instrucció: 9XY0
                                             
void inst_inicialitza_I(chip8_t* chip8);// Instrucció: ANNN
                                          
void inst_salt_registre(chip8_t* chip8);// Instrucció: BNNN 

void inst_byte_aleatori(chip8_t* chip8);// Instrucció: CXNN

void inst_pinta_sprite(chip8_t* chip8);// Instrucció: DXYN

void inst_salta_igual_key(chip8_t* chip8);// Instrucció: EX9E
void inst_salta_diferent_key(chip8_t* chip8);// Instrucció: EXA1
                                             //
void inst_iguala_registre_timer(chip8_t* chip8);// Instrucció: F0X7
void inst_espera_key(chip8_t* chip8);// Instrucció: FX0A
void inst_estableix_delay_timer(chip8_t* chip8);// Instrucció: FX15
void inst_estableix_sound_timer(chip8_t* chip8);// Instrucció: FX18
void inst_index_mes_registre(chip8_t* chip8);// Instrucció: FX1E
void inst_I_per_sprite(chip8_t* chip8);// Instrucció: FX29
void inst_guarda_decimal(chip8_t* chip8);// Instrucció: FX33
void inst_carrega_memoria(chip8_t* chip8);// Instrucció: FX55
void inst_carrega_registres(chip8_t* chip8);// Instrucció: FX65 
#endif
