#include "sdlib.h"

int init_sdl(void){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0){
        SDL_Log("Unable to initialize SDL: %s \n", SDL_GetError());
        return 0;
    }
    return 1;
}

void final_cleanup(const sdl_w w){
    SDL_DestroyRenderer(w.renderer);
    SDL_DestroyWindow(w.window);
    SDL_Quit();
}


int window_Creator(sdl_w * w,const windowConfig config){

    w->window = SDL_CreateWindow("Emulador de Chip8",
                                            SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED,
                                            config.window_width*config.scale_factor,
                                            config.window_height*config.scale_factor,
                                            SDL_WINDOW_OPENGL);
    if(!w->window){
        SDL_Log("No s'ha pogut crear la finestra desitjada %s \n",SDL_GetError());
        return 0;
    }

    w->renderer =SDL_CreateRenderer(w->window,-1,SDL_RENDERER_ACCELERATED);

    if(!w->renderer){
        SDL_Log("No s'ha pogut renderitzar l'emulador: %s \n", SDL_GetError());
        return 0;
    }

    return 1;

}

int set_config_from_arg(windowConfig * config,const int argc,const char ** argv){

    //Default:
    config->window_width = 64;
    config->window_height = 32;
    config->fg_color = 0xFF0000FF; //
    config->bg_color = 0x000000FF; // 
    config->scale_factor = 20;

    // Override_defaults:
    for(int i = 1; i < argc; i++){
        (void)argv[i];
        
    }
    return 1;
}
void mode_background(sdl_w *w, const windowConfig config){
    const uint8_t r = (config.bg_color >> 24) & 0xFF;
    const uint8_t g = (config.bg_color >> 16) & 0xFF;
    const uint8_t b = (config.bg_color >> 8) & 0xFF;
    const uint8_t a = (config.bg_color >> 0) & 0xFF;
    SDL_SetRenderDrawColor(w->renderer,r,g,b,a);

}

void mode_foreground(sdl_w *w, const windowConfig config){
    const uint8_t r = (config.fg_color >> 24) & 0xFF;
    const uint8_t g = (config.fg_color >> 16) & 0xFF;
    const uint8_t b = (config.fg_color >> 8) & 0xFF;
    const uint8_t a = (config.fg_color >> 0) & 0xFF;
    SDL_SetRenderDrawColor(w->renderer,r,g,b,a);

}

void update_screen(sdl_w *w, uint8_t display[],const windowConfig config){
    
        //Es comprova cada pixel de pantalla si cal pintar-lo.
    for (int i =0; i < 64*32 ; i++){

        //Posició a 64x32:
        int x = i % 64;
        int y = i / 64;
        //Posició a resolució
        x = x * config.scale_factor;
        y = y * config.scale_factor;
        //Objecte Rectangle.

        SDL_Rect rect= {
            .x = x,
            .y= y,
            .w = config.scale_factor,
            .h = config.scale_factor
        };
        if(display[i]){
            //Canvia a mode pintar pixels:
            mode_foreground(w,config);
            //Es passen les dades al renderer.
            SDL_RenderDrawRect(w->renderer,&rect);
            SDL_RenderFillRect(w->renderer, &rect);
        }else{
            //Canvia a mode pintar background:
            mode_background(w,config);
            //Es passen les dades al renderer.
            SDL_RenderDrawRect(w->renderer,&rect);
            SDL_RenderFillRect(w->renderer, &rect);
        }
    }
    //Es volquen els canvis.
    SDL_RenderPresent(w->renderer);
}

void paint_background(sdl_w *w, const windowConfig config){
    mode_background(w,config);
    SDL_RenderFillRect(w->renderer,NULL);

}

void clear_screen(sdl_w *w,const windowConfig config){
    
    //Neteja de Pantalla inicial:
    SDL_RenderClear(w->renderer);
    //Pintem Background de nou:
    paint_background(w,config);
    //Efectuem canvis al renderer.    
    SDL_RenderPresent(w->renderer);
}
