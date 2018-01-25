#include "SourceOverlays/Overlays.h"
#include "SourceStates/game_state.h"
#include "SourceCore/colide.h"

OverlayExitMenu::OverlayExitMenu()
{
    hits.push_back(new HitBox(-camW / 2,-camH / 2,camW,camH,camW / 2,camH / 2,0));
    hitsInclusive.push_back(true);
    bkg = new Image(-300,-350,600,700,0,"Overlays/exit_menu.png",camW / 2,camH / 2);
    type = OverlayType::EXIT;
    nameTex = new Text(1000,80,(camW / 2) - 300 + 90,(camH / 2) - 350 + 50,"ESC",0,0,0,false);
    buttons.push_back(new Text(1000,60,(camW / 2) - 300 + 70,(camH / 2) - 350 + 180,"Resume",0,0,0,false));
    buttons.push_back(new Text(1000,60,(camW / 2) - 300 + 70,(camH / 2) - 350 + 260,"Main Menu",0,0,0,false));
    buttons.push_back(new Text(1000,60,(camW / 2) - 300 + 70,(camH / 2) - 350 + 340,"Quit",0,0,0,false));
}

void OverlayExitMenu::render()
{
    render_rect(0,0,camW,camH,0,0,0,.75);
    bkg->render();
    nameTex->render();
    for(int i = 0;i < buttons.size();i++)
    {
        buttons[i]->render();
    }
}

bool OverlayExitMenu::handle_click()
{
    for(int i = 0;i < buttons.size();i++)
    {
        if(lp_point(mousePos,*buttons[i]->texCoords))
        {
            if(i == 0)
            {
                subMain->toggle_overlay(OverlayType::EXIT);
            }
            else if (i == 1)
            {
                active = &menuMain;
            }
            else if(i == 2)
            {
                running = false;
            }
        }
    }
    return false;//hit box is all inclusive
}




































