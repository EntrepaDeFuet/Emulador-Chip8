#include<stdio.h>
#include<SDL2/SDL.h>
#include "emulator.h"
#include<time.h>




int main(int argc, char **argv){
    (void) argc;
    (void) argv;

    srand(time(NULL));
    // Incialitzar SDL:  
    if(!init_sdl()) exit(EXIT_FAILURE);


    //Inicialitzem l'objecte emulador:
    chip8_t chip8 = {0};
    
    //const char nom[] = argv[1]; No implementat yet.
    if(argc < 2){
        if(!init_chip8(&chip8,"IBMLogo.ch8")) exit(EXIT_FAILURE);
    }
    else{
        if(!init_chip8(&chip8,argv[1])) exit(EXIT_FAILURE);
    }


    //Finestra Inicial:
    windowConfig config = {0};
    if(!set_config_from_arg(&config,argc,(const char**)argv)) exit(EXIT_FAILURE);

    sdl_w finestra = {0};

    if(!window_Creator(&finestra,config)) exit(EXIT_FAILURE);

   
    //Bucle principal del emulador:
    //
    clear_screen(&finestra,config);

    while(chip8.state != QUIT){
        //Controla inputs    
        handle_input(&chip8);
        //Finalitza el bucle
        if (chip8.state == QUIT) break;
        while(chip8.state == PAUSED) continue;
        //Executa instruccio
        emulate_instruction(&chip8);

        //Update Window
        //clear_screen(&finestra, config); // Diria que no fa falta.
        update_screen(&finestra,chip8.display,config);
    
        //Delay 60hz
        SDL_Delay(16);
        
           

    }

    


    //Finalitzar SDL
    final_cleanup(finestra); 

    exit(EXIT_SUCCESS);
}
