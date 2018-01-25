#ifndef START_UP_H
#define START_UP_H

#include "SDL.h"
#include "SDL_opengl.h"

void init();
void load_files();
void handle_window();
int load_text(SDL_Surface *);

#endif // START_UP_H
