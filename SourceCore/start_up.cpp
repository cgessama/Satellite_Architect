#include "start_up.h"
#include "SourceCore/consts.h"
#include "SourceCore/engine.h"
#include "SourceCore/globals.h"
#include "SourceStates/states.h"
#include "SourceStates/game_state.h"

void init()
{
    std::ifstream resolution;
    std::string data;
    resolution.open("resolution.txt");
    std::getline(resolution,data);
    windowW = string_to_int(data);
    windowH = windowW * (9.0 / 16.0);
    resolution.close();
    //^window stuff^
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,1);
    gWindow = SDL_CreateWindow("Satellite Architect",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,windowW,windowH,SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    gContext = SDL_GL_CreateContext(gWindow);
    handle_window();
    glClearColor(0,0,0,1);
    glEnable(GL_TEXTURE_2D);
    //alpha enable
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void load_files()
{
    dataFont = new Font("data");
    //SDL_ShowCursor(0);
    gameMain = new GameState();//this is up here so it already points when it is assigned to play button
    menuLoad = new MenuLoad;

    menuMain.bkg = new Image(-camW / 2,-camH / 2,camW,camH,0,"menu_bkg.png",camW / 2,camH / 2);
    menuMain.buttons.push_back(new Button());
    menuMain.buttons[0]->tex = new Text(300,100,250,600,"Play",255,0,0,false);
    menuMain.buttons[0]->effect = menuLoad;
    menuMain.buttons.push_back(new Button());
    menuMain.buttons[1]->tex = new Text(300,100,250,850,"Settings",255,0,0,false);
    menuMain.buttons[1]->effect = &menuSettings;

    menuSettings.bkg = new Image(-camW / 2,-camH / 2,camW,camH,0,"Settings.png",camW / 2,camH / 2);
    menuSettings.keyComs.push_back(new KeyCom());
    menuSettings.keyComs[0]->key = SDLK_BACKSPACE;
    menuSettings.keyComs[0]->effect = &menuMain;
}

void handle_window()
{
    if(windowW / windowH > windowAspectRatio)//window is wider than it is tall
    {
        windowOffX = (windowW - (windowAspectRatio * windowH)) / 2;
        windowOffY = 0;
        viewW = windowH * windowAspectRatio;
        viewH = windowH;
    }
    else
    {
        windowOffX = 0;
        windowOffY = (windowH - ((1 / windowAspectRatio) * windowW)) / 2;
        viewW = windowW;
        viewH = windowW * (1 / windowAspectRatio);
    }
    glViewport(windowOffX,windowOffY,viewW,viewH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,camW,camH,0,1,-1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

