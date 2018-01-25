#include "SourceStates/states.h"
#include "SourceCore/colide.h"
#include "SourceCore/globals.h"

MenuState menuMain;
MenuState menuSettings;

MenuState::MenuState()
{
}

void MenuState::handle_events()
{
    if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        for(int i = 0;i < buttons.size();i++)
        {
            if(lp_point(mousePos,*buttons[i]->tex->texCoords))
            {
                active = buttons[i]->effect;
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

void MenuState::logic()
{
}

void MenuState::render()
{
    bkg->render();
    for(int i = 0;i < buttons.size();i++)
    {
        buttons[i]->tex->render();
    }
}
