#include "SourceCore/start_up.h"
#include "SourceStates/states.h"
#include "SourceCore/engine.h"
#include "SourceCore/globals.h"
#include "SourceCore/consts.h"
#include "SourceGame/subMain.h"

void main_loop()
{
    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = false;
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEMOTION)
            {
                int x,y;
                SDL_GetMouseState(&x,&y);
                x = (x - windowOffX) * (camW / viewW);
                y = (y - windowOffY) * (camH / viewH);
                mousePos = Point(x,y);
            }
            else if(event.type == SDL_WINDOWEVENT && event.window.event ==  SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                windowW = event.window.data1;
                windowH = event.window.data2;
                handle_window();
                subMain->update_window();
            }
            active->handle_events();
        }
        active->logic();
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        active->render();
        SDL_GL_SwapWindow( gWindow );
        while(SDL_GetTicks() < double(lastFrame) + (1000.0 / double(FPS)))
        {
            SDL_Delay(5);
        }
        lastFrame = SDL_GetTicks();
        frameCount++;
    }
}

int main(int argc,char * args[])
{
    init();
    load_files();
    active = &menuMain;
    main_loop();
    std::ofstream resolution;
    resolution.open("resolution.txt", std::ios::trunc);
    resolution << viewW;
    resolution.close();
    SDL_Quit();
    return 0;
}




