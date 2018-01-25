#include "SourceCore/globals.h"
#include "SourceCore/engine.h"

float windowW = 0;
float windowH = 0;
float viewW = 0;
float viewH = 0;
float windowOffX = 0;
float windowOffY = 0;
float camW = 1920;
float camH = 1080;

Uint32 lastFrame = 0;
Uint32 frameCount = 0;
SDL_Window * gWindow = NULL;
SDL_GLContext gContext;
//input
bool running = true;
SDL_Event event;

std::vector<Picture *> pics;
Font * dataFont = NULL;
Point mousePos(0,0);

