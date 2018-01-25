#include "SourceGame/subMain.h"
#include "SourceStates/game_state.h"
#include "SourceModule/modules.h"
#include "SourceOverlays/Overlays.h"
#include "SourceAstronauts/astronaut.h"
#include "SourceStations/stations.h"
#include "SourceAstronauts/team.h"
#include "SourceResearch/Option.h"
#include "SourceMission/Mission.h"


SubMain::SubMain()
{
    sTGOrbit = new STGOrbit;
    sTGGround = new STGGround;
    station = NULL;

    noPlaceBool = false;
    showOverlays = true;
    inHandTechRot = 0;
    inHandA = NULL;
    overlays.push_back(new OverlayInfoBanner);
    overlays.push_back(new OverlayEditor);
    overlays.push_back(new OverlayManager);
    overlays.push_back(new OverlayDetailPanel(0,0,100,{255,0,0},{0,255,0},"Oxygen",OverlayType::OXYGEN));
    overlays.push_back(new OverlayDetailPanel(1,0,35,{0,0,255},{255,0,0},"Temperature",OverlayType::TEMPERATURE));
    overlays.push_back(new OverlayDetailPanel(2,0,1,{255,0,0},{0,255,0},"Integrity",OverlayType::INTEGRITY));
    overlays.push_back(new OverlayEnergyInfo);
    overlays.push_back(new OverlayExitMenu);//OverlayType
}

void SubMain::handle_events()
{
    if(event.type == SDL_KEYUP)
    {
        handle_key_up();
    }
    else if(event.type == SDL_MOUSEBUTTONDOWN)
    {
        if(event.button.button == SDL_BUTTON_LEFT)
        {
            if(find_overlay(OverlayType::EXIT) != -1)
            {
                overlays[OverlayType::EXIT]->handle_click();
            }
            else
            {
                handle_click();
            }
        }
        else if(event.button.button == SDL_BUTTON_RIGHT && find_overlay(OverlayType::EXIT) == -1)
        {
            for(int i = 0;i < selected.size();i++)
            {
                selected[i]->selectedi->stored = false;
            }
            selected.clear();
        }
    }
    else if(event.type == SDL_MOUSEWHEEL && find_overlay(OverlayType::EXIT) == -1)
    {
        Point temp = station->gameSpace->orig - gameMousePos;
        if(event.wheel.y > 0)
        {
            station->gameSpaceScale *= .95;
            station->gameSpace->scale(.95);
            temp = temp * .95;
        }
        else
        {
            station->gameSpaceScale *= 1.05;
            station->gameSpace->scale(1.05);
            temp = temp * 1.05;
        }
        if(station->gameSpaceScale < .1)
        {
            float temp2 = .1 / station->gameSpaceScale;
            station->gameSpaceScale *= temp2;
            station->gameSpace->scale(temp2);
            temp = temp * temp2;
        }
        if(station->gameSpaceScale > 10)
        {
            float temp2 = 10 / station->gameSpaceScale;
            station->gameSpaceScale *= temp2;
            station->gameSpace->scale(temp2);
            temp = temp * temp2;
        }
        station->gameSpace->orig = gameMousePos + temp;//zoom into cursor
    }
}

void SubMain::logic()
{
    if(find_overlay(OverlayType::EXIT) == -1)
    {
        key_logic();
        calc_mouse_pos();
        for(int i = 0;i < activeOverlays.size();i++)
        {
            activeOverlays[i]->logic();
        }
        station->logic();
        for(int i = 0;i < station->teams.size();i++)
        {
            station->teams[i]->logic();
        }
        if(subResearch->queue.size() != 0)
        {
            subResearch->queue[0]->eval_completion();
        }
        else
        {
            if(gameMain->timeAccel != 0)
            {
                gameMain->timeAccel = 0;
                create_popups("Please choose a research");
            }
        }
        for(int i = 0;i < subMission->missions.size();i++)
        {
            subMission->missions[i]->eval();
        }
    }
}

void SubMain::render()
{
    glLoadIdentity();
    //bkg
    station->stage->render_bkg();
    //modules layer
    glLoadIdentity();
    glTranslatef(camW / 2,camH / 2,0);
    glScalef(1 / station->gameSpaceScale,1 / station->gameSpaceScale,1);
    glRotatef(-station->gameSpace->rot,0,0,1);
    glTranslatef(-station->gameSpace->orig.x,-station->gameSpace->orig.y,0);
    for(int i = 0;i < station->modules.size();i++)
    {
        if(!station->modules[i]->inHandi->stored)//this one is not inhand
        {
            station->modules[i]->render();
        }
    }
    for(int i = 0;i < selected.size();i++)
    {
        if(selected[i]->inHandi->stored || find_overlay(OverlayType::EDITOR) == -1)//these are inhand
        {
            break;
        }
        selected[i]->draw_outline(1,1,0,1,.02);
    }
    for(int i = 0;i < station->teams.size();i++)
    {
        if(station->teams[i] != inHandA)//astronaut is not inHand
        {
            if(station->teams[i]->inside != NULL)
            {
                if(!station->teams[i]->inside->inHandi->stored)//the modules it is inside is not inhand
                {
                    station->teams[i]->render();
                }
            }
            else//inside nothing
            {
                station->teams[i]->render();
            }
        }
    }
    //inHand layer
    for(int i = 0;i < inHand.size();i++)
    {
        inHand[i]->render();
        if(inHand[i]->contents != NULL)
        {
            inHand[i]->contents->render();
        }
    }
    for(int i = 0;i < inHand.size();i++)
    {
        if(inHand[i]->selectedi->stored)
        {
            inHand[i]->draw_outline(1,1,0,1,.02);
        }
    }
    if(inHandA != NULL)
    {
        inHandA->render();
    }
    if(noPlaceBool)
    {
        render_no_place();
    }
    //UI layer
    glLoadIdentity();
    if(activeOverlays.size() != 0)//there is a UI open
    {
        for(int i = activeOverlays.size() - 1;i >= 0;i--)//render backwards so that top to bottm lines up with logic
        {
            activeOverlays[i]->render();
        }
    }
    show_tool_tip();
    show_popups();
}

void SubMain::change_station(int s)
{
    bool cont = true;
    if(subMain->inHand.size() != 0)
    {
        subMain->create_popups("Please Place Modules Before Changing Stations");
        cont = false;
    }
    if(subMain->inHandA != NULL)
    {
        subMain->create_popups("Please Place Astronauts Before Changing Stations");
        cont = false;
    }
    if(cont)
    {
        station = stations[s];
    }
}

