#include "SourceStates/states.h"
#include "SourceGame/subMain.h"
#include "SourceStates/game_state.h"
#include "SourceCore/colide.h"

MenuLoad * menuLoad;

MenuLoad::MenuLoad()
{
    std::ifstream file("saves/saveData.txt");
    std::string line;
    int i = 0;
    while(std::getline(file,line))
    {
        fileNames.push_back(line);
        fileTexes.push_back(new Text(1000,50,fileTexesOffX,fileTexesOffY + (i * fileTexesRelativeOffY),line,255,0,0,false));
        i++;
    }
    bkg = new Image(-camW / 2,-camH / 2,camW,camH,0,"loadbkg.png",camW / 2,camH / 2);
    keyComs.push_back(new KeyCom());
    keyComs[0]->key = SDLK_BACKSPACE;
    keyComs[0]->effect = &menuMain;
}

void MenuLoad::handle_events()
{
    if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        for(int i = 0;i < fileTexes.size();i++)
        {
            if(lp_point(mousePos,*fileTexes[i]->texCoords))
            {
                active = gameMain;
                gameMain->load_save(fileNames[i]);
                break;
            }
        }
    }
    if(event.type == SDL_KEYDOWN)
    {
        for(int i = 0;i < keyComs.size();i++)
        {
            if(event.key.keysym.sym == keyComs[0]->key)
            {
                active = keyComs[i]->effect;
            }
        }
    }
}

void MenuLoad::logic()
{

}

void MenuLoad::render()
{
    bkg->render();
    for(int i = 0;i < fileTexes.size();i++)
    {
        fileTexes[i]->render();
    }
    for(int i = 0;i < buttons.size();i++)
    {
        buttons[i]->tex->render();
    }
}




























































