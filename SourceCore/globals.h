#ifndef GLOBALS_H
#define GLOBALS_H
#include "SDL.h"
#include "SDL_opengl.h"
#include <vector>

class Point;
class HitBox;
class Picture;
class Font;

//real globals
extern float windowW;//dimensions of window
extern float windowH;
extern float viewW;//physical dimensions of rendered area (pixels)
extern float viewH;
extern float windowOffX;//distance of rendered area from the side of the screen
extern float windowOffY;
extern float camW;//virtual rendered area dimensions
extern float camH;

extern Uint32 lastFrame;
extern Uint32 frameCount;
extern SDL_Window* gWindow;
extern SDL_GLContext gContext;
//input
extern bool running;
extern SDL_Event event;

extern std::vector<Picture *> pics;
extern Font * dataFont;
extern Point mousePos;


#endif // GLOBALS_H
