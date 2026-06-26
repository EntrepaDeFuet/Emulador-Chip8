#include"emulator.h"

int init_chip8(chip8_t * chip8,const char rom_name[]){
    const uint32_t entrada_ram = 0x200;
    const uint8_t font[] = {
        
        0xF0, 0x90, 0x90, 0x90, 0xF0, //0 
        0x20, 0x60, 0x20, 0x20, 0x70, //1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
                                          
        0xA0, 0xA0, 0xF0, 0x20, 0x20, //4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
        0xF0, 0x10, 0x10, 0x10, 0x10, //7
                                         
        0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
        0xF0, 0x90, 0xF0, 0x90, 0x90, //A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
                                         
        0xF0, 0x80, 0x80, 0x80, 0xF0, //C
        0xE0, 0x90, 0x90, 0x90, 0xE0, //D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
        0xF0, 0x80, 0xF0, 0x80, 0x80, //F
                                         
                                        
    };
    //LOAD FONT:
    memcpy(&chip8->ram[0],font,sizeof(font));

    //LOAD ROM:
    FILE *rom = fopen(rom_name,"rb");
    if (!rom){
        SDL_Log("No s'ha trobat el ROM: %s\n",rom_name);
        return 0;
    }
    //Compta el número de bytes a llegir del arxiu per tal d'extreure'n el tamany.
    fseek(rom,0,SEEK_END);
    const long rom_size = ftell(rom);
     
    //Calcul de l'espai disponible de memòria.
    const size_t max_size = sizeof(chip8->ram) - entrada_ram;

    //Retorna a l'inici del arxiu.
    rewind(rom);
    if(rom_size > (long) max_size){
        SDL_Log("La mida del ROM: %s és massa gran\n",rom_name);
        return 0;
    }

    //Llegir el ROM:
    if(!fread(&chip8->ram[entrada_ram], rom_size, 1, rom)){
        SDL_Log("No s'ha pogut carregar el ROM a memòria.\n");
        return 0;
    }
    
    fclose(rom);


    //Neteja de memòria del display.
    inst_neteja_pantalla(chip8);
    //Neteja de flags de keypad.
    chip8->keypad = 0x0000;
    //Inicialització del program counter.
    chip8->pc = entrada_ram;
    chip8->sp = &chip8->stack[0];
    chip8->state = RUNNING;
    chip8->keypad = 0x00;
    return 1;
    ;
}

void handle_input(chip8_t * chip8){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){

            case SDL_QUIT:
                chip8->state = QUIT;
                return;
            //Lectura de tecles.
            case  SDL_KEYDOWN:

                switch (event.key.keysym.sym){

                      case SDLK_ESCAPE :
                            chip8->state = QUIT;
                            return;
                      case SDLK_SPACE:
                            if(chip8->state == PAUSED) chip8->state = RUNNING;
                            else chip8->state = PAUSED;
                            return;
                      case SDLK_1:
                            chip8->keypad |= (1<<0);
                            break;
                      case SDLK_2:
                            chip8->keypad |= (1<<1);
                            break;
                      case SDLK_3:
                            chip8->keypad |= (1<<2);
                            break;
                      case SDLK_4:
                            chip8->keypad |= (1<<3);
                            break;
                      case SDLK_q:
                            chip8->keypad |= (1<<4);
                            break;
                      case SDLK_w:
                            chip8->keypad |= (1<<5);
                            break;
                      case SDLK_e:
                            chip8->keypad |= (1<<6);
                            break;
                      case SDLK_r:
                            chip8->keypad |= (1<<7);
                            break;
                      case SDLK_a:
                            chip8->keypad |= (1<<8);
                            break;
                      case SDLK_s:
                            chip8->keypad |= (1<<0x9);
                            break;
                      case SDLK_d:
                            chip8->keypad |= (1<<0xa);
                            break;
                      case SDLK_f:
                            chip8->keypad |= (1<<0xb);
                            break;
                      case SDLK_z:
                            chip8->keypad |= (1<<0xc);
                            break;
                      case SDLK_x:
                            chip8->keypad |= (1<<0xd);
                            break;
                      case SDLK_c:
                            chip8->keypad |= (1<<0xe);
                            break;
                      case SDLK_v:
                            chip8->keypad |= (1<<0xf);
                            break;
                      default:
                            break;
                }

                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym){

                      
                      case SDLK_1:
                            chip8->keypad &= (0<<0);
                            break;
                      case SDLK_2:
                            chip8->keypad &= (0<<1);
                            break;
                      case SDLK_3:
                            chip8->keypad &= (0<<2);
                            break;
                      case SDLK_4:
                            chip8->keypad &= (0<<3);
                            break;
                      case SDLK_q:
                            chip8->keypad &= (0<<4);
                            break;
                      case SDLK_w:
                            chip8->keypad &= (0<<5);
                            break;
                      case SDLK_e:
                            chip8->keypad &= (0<<6);
                            break;
                      case SDLK_r:
                            chip8->keypad &= (0<<7);
                            break;
                      case SDLK_a:
                            chip8->keypad &= (0<<8);
                            break;
                      case SDLK_s:
                            chip8->keypad &= (0<<0x9);
                            break;
                      case SDLK_d:
                            chip8->keypad &= (0<<0xa);
                            break;
                      case SDLK_f:
                            chip8->keypad &= (0<<0xb);
                            break;
                      case SDLK_z:
                            chip8->keypad &= (0<<0xc);
                            break;
                      case SDLK_x:
                            chip8->keypad &= (0<<0xd);
                            break;
                      case SDLK_c:
                            chip8->keypad &= (0<<0xe);
                            break;
                      case SDLK_v:
                            chip8->keypad &= (0<<0xf);
                            break;
                      default:
                            break;
                }
                break;
            default:
                break;
        }
    }

}
void emulate_instruction(chip8_t *chip8){

    chip8->actual.opcode = (chip8->ram[chip8->pc] << 8) | (chip8->ram[chip8->pc+1]);
    chip8->pc +=2; // Seguent opcode.
                   
    //S'extreu del opcode cadascun dels valors a partir de màscares:
    chip8->actual.cat =(uint8_t) ((chip8->actual.opcode & 0xf000)>>12 );
    chip8->actual.NNN = (chip8->actual.opcode) & 0x0fff;
    chip8->actual.NN =(uint8_t)(chip8->actual.opcode & 0x00ff);
    chip8->actual.N = (uint8_t)(chip8->actual.opcode & 0x000f);
    //Es mouen a la dreta per evitar ser retallades:
    chip8->actual.X = (uint8_t)((chip8->actual.opcode & 0x0f00)>>8);
    chip8->actual.Y = (uint8_t)((chip8->actual.opcode & 0x00f0)>>4);

   //switch d'opcodes:
   switch (chip8->actual.cat) {
        case 0x00:
            if(chip8->actual.NN == 0xE0) inst_neteja_pantalla(chip8);
            else if(chip8->actual.NN == 0xEE) inst_retorn_subrutina(chip8);
            break;
        case 0x01:

            inst_salt(chip8);
            break;
        case 0x02:
            inst_entrar_subrutina(chip8);
            break;
        case 0x03:
            inst_next_igual(chip8);
            break;
        case 0x04:
            inst_next_diferent(chip8);
            break;
        case 0x05:
            inst_next_igual(chip8);
            break;
        case 0x06:
            inst_inicialitzar_registre_immediat(chip8);
            break;
        case 0x07:
            inst_suma_immediat(chip8);
            break;
        case 0x08:
            switch(chip8->actual.N){
                case 0x0:
                    inst_igualar_registre(chip8);
                    break;
                case 0x1:
                    inst_or_registre(chip8);
                    break;
                case 0x2:
                    inst_and_registre(chip8);
                    break;
                case 0x3:
                    inst_xor_registre(chip8);
                    break;
                case 0x4:
                    inst_suma_registre(chip8);
                    break;
                case 0x5:
                    inst_resta_registre(chip8);
                    break;
                case 0x6:
                    inst_dividir_entredos(chip8);
                    break;
                case 0x7:
                    inst_resta_inversa(chip8);
                    break;
                case 0xE:
                    inst_multiplica_perdos(chip8);
                    break;
                default:
                    break;
            }
            break;
        case 0x09:
            inst_comparar_registres(chip8);
            break;
        case 0x0A:
            inst_inicialitza_I(chip8);
            break;
        case 0x0B:
            inst_salt_registre(chip8);
            break;
        case 0x0C:
            inst_byte_aleatori(chip8);
            break;
        case 0x0D:
            inst_pinta_sprite(chip8);
            break;
        case 0x0E:
            switch(chip8->actual.N){
                case 0xE:
                    inst_salta_igual_key(chip8);
                    break;
                case 0x1:
                    inst_salta_diferent_key(chip8);
                    break;
                default:
                    break;
            }
            break;
        case 0x0F:
            switch(chip8->actual.N){
                case 0x7:
                    inst_iguala_registre_timer(chip8);
                    break;
                case 0xA:
                    inst_espera_key(chip8);
                    break;
                case 0x5:
                    switch(chip8->actual.NN){
                        case 0x15:
                            inst_estableix_delay_timer(chip8);
                            break;
                        case 0x55:
                            inst_carrega_memoria(chip8);
                            break;
                        case 0x65:
                            inst_carrega_registres(chip8);
                            break;
                    }
                    break;
                case 0x8:
                    inst_estableix_sound_timer(chip8);
                    break;
                case 0xE:
                    inst_index_mes_registre(chip8);
                    break;
                case 0x9:
                    inst_I_per_sprite(chip8);
                    break;
                case 0x3:
                    inst_guarda_decimal(chip8);
                    break;
                default:
                     break;       
            }
            break;
        default:
            break;
   }




}

void inst_neteja_pantalla(chip8_t *chip8){
    //Tots els pixels de la pantalla a 0
    memset(&chip8->display[0],0,sizeof(chip8->display));
}
void inst_retorn_subrutina(chip8_t* chip8){
    //Decrementa el sp
    //Rescata el Return address del sp
    chip8->sp--;
    chip8->pc =* chip8->sp;
    
}
void inst_salt(chip8_t* chip8){
    //Estableix el pc a l'adreça NNN
    chip8->pc = chip8->actual.NNN & 0xFFF;
}
void inst_entrar_subrutina(chip8_t* chip8){
    //Guarda valor del pc a l'adreça del sp actual.
    //Carrega l'adreça NNN al pc
    //incrementa el sp.
    *chip8->sp = chip8->pc;
    chip8->pc = chip8->actual.NNN & 0x0FFF;
    chip8->sp++; 

}
void inst_next_igual(chip8_t *chip8){
    //Si VX == NN, PC +2
    if(chip8->v[chip8->actual.X] == chip8->actual.NN){
        chip8->pc+=2;
    }
}
void inst_next_diferent(chip8_t* chip8){
    //Si Vx != NN, PC +2
    if(chip8->v[chip8->actual.X] != chip8->actual.NN) chip8->pc +=2;
}
void inst_next_igual_registres(chip8_t* chip8){
    //Si Vx == Vy, PC +2
    if(chip8->v[chip8->actual.X] == chip8->v[chip8->actual.Y]) chip8->pc +=2;
}
void  inst_inicialitzar_registre_immediat(chip8_t *chip8){
    // Inicialitza Vx = NN
    chip8->v[chip8->actual.X] = chip8->actual.NN;
}

void inst_suma_immediat(chip8_t* chip8){
    // Vx += NN
    chip8->v[chip8->actual.X] += chip8->actual.NN;
}

void inst_igualar_registre(chip8_t* chip8){
    // Vx = Vy
    chip8->v[chip8->actual.X] = chip8->v[chip8->actual.Y];
}
void inst_or_registre(chip8_t* chip8){
    //Vx = Vx or Vy
    chip8->v[chip8->actual.X] |= chip8->v[chip8->actual.Y];
}
void inst_and_registre(chip8_t* chip8){
    //Vx = Vx and Vy
    chip8->v[chip8->actual.X] &= chip8->v[chip8->actual.Y];
}
void inst_xor_registre(chip8_t* chip8){
    //Vx = Vx xor Vy
    chip8->v[chip8->actual.X] ^= chip8->v[chip8->actual.Y];
}
void inst_suma_registre(chip8_t* chip8){
    //Vx = Vx + Vy
    //Si hi ha carry Vf =1; Si no hi ha carry, Vf = 0M.
    uint16_t suma = chip8->v[chip8->actual.X] + chip8->v[chip8->actual.Y];
    chip8->v[chip8->actual.X]  = (uint8_t) (suma & 0XFF);
    chip8->v[0xF] = (uint8_t) (suma >> 8);

}   
void inst_resta_registre(chip8_t * chip8){
    //Si Vx < Vy -> Vf = 1;
    //Vx = Vx - Vy
    if(chip8->v[chip8->actual.X] < chip8->v[chip8->actual.Y]) chip8->v[0xF] = 0;
    else chip8->v[0xF] = 1; 
    chip8->v[chip8->actual.X] -= chip8->v[chip8->actual.Y];
}
void inst_dividir_entredos(chip8_t* chip8){
    //Si Vx % 2 == 1 -> Vf = 1;
    // Vx = Vx/2
    chip8->v[0xF] = chip8->v[chip8->actual.X] & (0x1);
    chip8->v[chip8->actual.X] >>= 1;    
}
void inst_resta_inversa(chip8_t * chip8){
    //Si Vy > Vy -> Vf = 1
    //Vx = Vy - Vx
    if(chip8->v[chip8->actual.X] < chip8->v[chip8->actual.Y]) chip8->v[0xf] = 1;
    chip8->v[chip8->actual.X] = chip8->v[chip8->actual.Y] - chip8->v[chip8->actual.X];
}

void inst_multiplica_perdos(chip8_t * chip8){
    // Si VX > 0x80, Vf = 1
    // Vx  =  Vx * 2
    if(chip8->v[chip8->actual.X] >= 0x80) chip8->v[0xF] = 1;
    chip8->v[chip8->actual.X] <<= 1;
}
void inst_comparar_registres(chip8_t* chip8){
    //Si Vx != Vy -> Salta a la següent instrucció
    if(chip8->v[chip8->actual.X]!= chip8->v[chip8->actual.Y]) chip8->pc+=2;
}
void inst_inicialitza_I(chip8_t* chip8){
    // Index pren valor NNN
    chip8->I = chip8->actual.NNN;
}
void inst_salt_registre(chip8_t * chip8){
    // PC = V0 + NNN
    chip8->pc = chip8->v[0] + chip8->actual.NNN; 
}
void inst_byte_aleatori(chip8_t* chip8){
    // Vx = Random & NN
    uint16_t random = (rand() & 0xFF) | ((rand() & 0xFF) << 8);
    chip8->v[chip8->actual.X] = random & chip8->actual.NN;
}   
void inst_pinta_sprite(chip8_t* chip8){
    // Dibuixa sprite d'alçada N, a les coordenades X,y
    //Vf, el carry es posa a 1 si algun pixel s'apaga.
    uint8_t X_coord = chip8->v[chip8->actual.X] % 64;
    uint8_t Y_coord = chip8->v[chip8->actual.Y] % 32;
    const uint8_t x_inicial = X_coord;
    chip8->v[0xF] = 0;

    for (uint8_t i =0 ; i< chip8->actual.N ; i++){
        //Es reinicia l'increment d'X per evitar escriure en escala.
        X_coord = x_inicial;

        // S'agafa la informació del sprite començant per la posicio I.
        const uint8_t sprite_data = chip8->ram[chip8->I+i];

        // Per cada bit de la llargada N, es comprova si s'ha d'encendre.
        for (int8_t j = 7; j >= 0; j--){

            //Es comprova si s'ha de setejar carry.
            if(sprite_data & (1<<j) && chip8->display[Y_coord * 64 + X_coord]){

                //Setejem Carry flag.
                chip8->v[0xF] = 1;
            }
            //Per cada bit de display, es fa XOR amb l'obtingut per memòria.
            chip8->display[Y_coord * 64 + X_coord] ^=
                (sprite_data & (1<<j)); //Només si el bit està encès.
                                        //
            //S'incrementa el valor d'X per escriure en horitzontal.    
            //Si s'arriba a la cantonada, surt del bucle d'escriptura horitzontal.
            if(++X_coord == 64) break; 
        }
        //S'incrementa Y per escriure de manera vertical.
        //Es controla també la cantonada vertical.
        if(++Y_coord == 32) break;

    }
}
void inst_salta_igual_key(chip8_t * chip8){
    //Si keypad & Vx -> salta a la següent instruccio
    uint8_t salts = chip8->v[chip8->actual.X] ;
    if(chip8->keypad & (1 << salts)) chip8->pc += 2;
}
void inst_salta_diferent_key(chip8_t * chip8){
    //Si la tecla Vx no està apretada, salta a la següent instrucció
    uint8_t salts = chip8->v[chip8->actual.X];
     if(!(chip8->keypad & (1<< salts))) chip8->pc += 2;
}
void inst_iguala_registre_timer(chip8_t * chip8){
    //Guarda a Vx el valor del delay timer.
    chip8->v[chip8->actual.X] = chip8->delay_timer;
}
void inst_espera_key(chip8_t* chip8){
    //El programa s'espera que el jugador premi una tecla
    //Guarda el valor de la tecla a Vx.
    static uint8_t boto_apretat = 0;
    static uint8_t boto = 0xff;
    //Per cada tecla reviso si ha estat anteriorment apretada i si ara ja no ho esta
    for (uint8_t i = 0; i<16 ; i++){
        if(chip8->keypad & (i<<i)){
            boto_apretat = 1;
            boto = i;
            break;
        }
    }
    if(!boto_apretat) chip8->pc -= 2;
    else{
        if(chip8->keypad & (1 << boto)) chip8->pc -=2;
        else {
            chip8->v[chip8->actual.X] = boto;
            boto = 0xff;
            boto_apretat =0;
        }
    }
}
void inst_estableix_delay_timer(chip8_t* chip8){
    // delay timer = VX
    chip8->delay_timer = chip8->v[chip8->actual.X];
}
void inst_estableix_sound_timer(chip8_t* chip8){
    // sound timer = VX
    chip8->sound_timer = chip8->v[chip8->actual.X];
}
void inst_index_mes_registre(chip8_t * chip8){
    // I = I + Vx
    chip8->I += chip8->v[chip8->actual.X];
}
void inst_I_per_sprite(chip8_t * chip8){
    //Mou el punter I en funció del valor registrat a Vx.
    chip8->I = chip8->v[chip8->actual.X] * 5; 
    
}
void inst_guarda_decimal(chip8_t * chip8){
    //Primer divideixo entre 100:
    uint8_t valor = chip8->v[chip8->actual.X];
    uint8_t xifra = valor / 100;
    //Registro el valor a l'adreça I
    chip8->ram[chip8->I] = xifra;
    //resto a valor les centessimes:
    valor = valor - (xifra*100);
    //Repeteixo
    xifra = valor/10;
    chip8->ram[chip8->I + 1] = xifra;

    valor = valor - (xifra*10);

    chip8->ram[chip8->I+2] = valor;
}

void inst_carrega_memoria(chip8_t * chip8){
    //V0 - Vx es carreguen a diferents espais de memòria començant per I.
    for(uint8_t i = 0; i <= chip8->actual.X ; i++){
        chip8->ram[chip8->I ++] = chip8->v[i];
    }
}
void inst_carrega_registres(chip8_t * chip8){
    //I - I+X es carreguen a V0- Vx
    for(uint8_t i = 0; i <= chip8->actual.X ; i++){
      chip8->v[i] = chip8->ram[chip8->I ++];
    }
}









