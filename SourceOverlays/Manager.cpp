#include "SourceOverlays/Overlays.h"
#include "SourceAstronauts/astronaut.h"
#include "SourceStates/game_state.h"
#include "SourceAstronauts/team.h"
#include "SourceCore/colide.h"


OverlayManager::OverlayManager()
{
    bkg = new Image(-camW / 2,-camH / 2,camW,camH,0,"Overlays/astroEditor/AstroEditor.png",camW / 2,camH / 2);
    plusTex = new Image(-30,-30,60,60,0,"Overlays/astroEditor/plus.png",210,1030);
    minusTex = new Image(-30,-30,60,60,0,"Overlays/astroEditor/minus.png",290,1030);
    nameTex = new Text(1000,60,1420,990,"New Astronaut",0,0,0,false);
    sizeText = new Text(160,60,250,970,"Size: 1",255,0,0,true);
    teamSize = 1;
    hits.push_back(new HitBox(-85,-85,510,170,85,995,0));
    hitsInclusive.push_back(true);
    Buttons.push_back (new Image(-buttonDim / 4,-buttonDim / 2,buttonDim / 2,buttonDim,0,"astronaut/main.png",buttonOffX,camH - buttonOffY));
    ButtonsText.push_back (new Text(1000,30,15,922,"New Team",255,0,0,false));
    type = OverlayType::MANAGER;
    excludes.push_back(1);//no astro editor
}

bool OverlayManager::handle_click()
{
    if(lp_point(mousePos,*hits[0]) == hitsInclusive[0])
    {
        if(subMain->inHandA == NULL)//nothing in hand
        {
            for(int i = 0;i < Buttons.size();i++)
            {
                if(lp_point(mousePos,*Buttons[i]->texCoords))
                {
                    if(gameMain->money - (100 * teamSize) > -1)
                    {
                        Team * t = new Team(teamSize);
                        subMain->inHandA = t;
                        subMain->station->teams.push_back(t);
                        gameMain->money -= 100 * teamSize;
                        subMain->create_popups("New Team of " + num_to_string(teamSize) + ": -$" + num_to_string(teamSize * 100));
                    }
                    else//not enough money
                    {
                        subMain->create_popups("You need " + num_to_string((100 * teamSize) - gameMain->money) + " more dollars");
                    }
                }
            }
        }
        if(lp_point(mousePos,*plusTex->texCoords))
        {
            teamSize++;
            sizeText->change_text("Size: " + num_to_string(teamSize));
        }
        else if(teamSize != 1 && lp_point(mousePos,*minusTex->texCoords))
        {
            teamSize--;
            sizeText->change_text("Size: " + num_to_string(teamSize));
        }
        return false;//do not continue
    }
    return true;
}

bool OverlayManager::handle_key()
{
    if(event.key.keysym.sym == 127)//delete
    {
        gameMain->money += subMain->inHandA->members.size() * 100;
        subMain->create_popups("Team Sold: +$" + num_to_string(subMain->inHandA->members.size() * 100));
        delete subMain->inHandA;
        subMain->inHandA = NULL;
        return false;
    }
    return true;
}

void OverlayManager::logic()
{
    if(subMain->inHandA != NULL)
    {
        subMain->inHandA->move_to(subMain->gameMousePos.x,subMain->gameMousePos.y);
        if(gameMain->keyStates[SDL_SCANCODE_Q])
        {
            subMain->inHandA->rotate(subMain->windowRotSpeed);
        }
        if(gameMain->keyStates[SDL_SCANCODE_E])
        {
            subMain->inHandA->rotate(-1 * subMain->windowRotSpeed);
        }
        if(gameMain->keyStates[SDL_SCANCODE_X])
        {
            subMain->inHandA->rotate(subMain->inHandRotSpeed);
        }
        if(gameMain->keyStates[SDL_SCANCODE_Z])
        {
            subMain->inHandA->rotate(-1 * subMain->inHandRotSpeed);
        }
    }
}

void OverlayManager::render()
{
    base_render();
    for(int i = 0;i < Buttons.size();i++)
    {
        Buttons[i]->render();
        ButtonsText[i]->render();
    }
    nameTex->render();
    sizeText->render();
    plusTex->render();
    minusTex->render();
}




















