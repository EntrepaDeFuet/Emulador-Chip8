#ifndef SDLIB_H
#define SDLIB_H

#include<stdio.h>
#include<SDL2/SDL.h>

//Structs definits per tal de fer funcionar el programa:
typedef struct{
    uint32_t window_height;
    uint32_t window_width;
    uint32_t  fg_color; //RGBA8888
    uint32_t bg_color; //RGBA8888
    uint32_t scale_factor; //La resolució original de chip8 són 64x32
} windowConfig;


typedef struct{
    SDL_Window * window;
    SDL_Renderer * renderer;
    
}sdl_w;

int init_sdl(void);

void final_cleanup(const sdl_w w);

int window_Creator(sdl_w *w, const windowConfig config);

int set_config_from_arg(windowConfig *, const int argc, const char ** argv);

void mode_background(sdl_w *w, const windowConfig config);

void mode_foreground(sdl_w *w, const windowConfig config);

void update_screen(sdl_w *w, uint8_t display[],const windowConfig config);

void paint_background(sdl_w *w,const windowConfig config);

void clear_screen(sdl_w *w, const windowConfig config);


#endif
