#include "SourceStates/game_state.h"
#include "SourceModule/modules.h"
#include "SourceOverlays/Overlays.h"
#include "SourceAstronauts/astronaut.h"
#include "SourceStations/stations.h"
#include "SourceAstronauts/team.h"
#include "SourceOverlays/enums.h"
#include "SourceModule/detail_enums.h"
#include "SourceCore/colide.h"

void SubMain::handle_key_up()
{
    bool cont = true;
    for(int i = 0;i < activeOverlays.size();i++)
    {
        if(!activeOverlays[i]->handle_key())
        {
            cont = false;
            break;
        }
    }
    if(cont)//the event was not claimed so continue with globals
    {
        switch(event.key.keysym.sym)
        {
        case 27://escape
            toggle_overlay(OverlayType::EXIT);
            break;
        case 44://comma
            if(subMain->station->stage_type == 0)
            {
                subMain->change_station(1);
            }
            else
            {
                subMain->change_station(0);
            }
            break;
        case 104://h
            gameMain->save("NULL");
            break;
        case 105://i
            toggle_overlay(OverlayType::EDITOR);
            break;
        case 107://k
            toggle_overlay(OverlayType::MANAGER);
            break;
        case 108://l
            toggle_overlay(OverlayType::ENERGY);
            break;
        case 109://m
            gameMain->subActive = subMission;
            break;
        case 111://o
            if(subMain->station->stage_type == 1)
            {
                create_popups("Oxygen is irrelevant here");
            }
            else
            {
                toggle_overlay(OverlayType::OXYGEN);
            }
            break;
        case 114://r
            gameMain->subActive = subResearch;
            break;
        case 116://t
            if(subMain->station->stage_type == 1)
            {
                create_popups("Temperature is irrelevant here");
            }
            else
            {
                toggle_overlay(OverlayType::TEMPERATURE);
            }
            break;
        case 127://delete
            if(inHand.size() > 0)
            {
                delete_inHand();
            }
			else
			{
				destroy_selection();
			}
            break;
		case 99://c
			gameMain->money += 100;
			break;
        case SDLK_F1:
            showOverlays = !showOverlays;
            if(showOverlays)//overlays need to come back
            {
                for(int i = 0;i < previouslyActiveOverlays.size();i++)
                {
                    activeOverlays.push_back(overlays[previouslyActiveOverlays[i]]);
                }
                previouslyActiveOverlays.clear();
            }
            else//all must disappear
            {
                for(int i = 0;i < activeOverlays.size();i++)
                {
                    previouslyActiveOverlays.push_back(activeOverlays[i]->type);
                }
                activeOverlays.clear();
            }
        default:
            break;
        }
    }

}

void SubMain::handle_click()
{
    bool cont = true;
    for(int i = 0;i < activeOverlays.size();i++)
    {
		int start = activeOverlays.size();
        if(!activeOverlays[i]->handle_click())//the click was handled
        {
            cont = false;
			if(activeOverlays.size() == start)//make sure the number of overlays didn't change
				overlay_to_front(activeOverlays[i]->type);//
            break;
        }
    }
    if(cont)
    {
        if(find_overlay(OverlayType::EDITOR) != -1)//editor UI is open
        {
            if(inHand.size() == 0)//nothing inHand
            {
                for(int i = 0;i < subMain->station->modules.size();i++)
                {
                    if(lp_point(gameMousePos,*subMain->station->modules[i]->hit))
                    {
                        if(!subMain->station->modules[i]->selectedi->stored)
                        {
                            selected.push_back(subMain->station->modules[i]);
                            selected[selected.size() - 1]->selectedi->stored = true;
                            selected[selected.size() - 1]->selectedi->index = selected.size() - 1;
                        }
                        else
                        {
                            selected.erase(selected.begin() + subMain->station->modules[i]->selectedi->index);
                            subMain->station->modules[i]->selectedi->stored = false;
                            for(int j = 0;j < selected.size();j++)//fix indexes
                            {
                                selected[j]->selectedi->index = j;
                            }
                        }
                        break;
                    }
                }
            }
            else//there is somthing in Hand
            {
                handle_placement();
            }
        }
        else if(find_overlay(OverlayType::MANAGER) != -1)
        {
            if(inHandA != NULL)//there is an astronaut in hand
            {
                bool placed = false;
                for(int i = 0;i < subMain->station->modules.size();i++)
                {
                    if(lp_point(subMain->gameMousePos,*subMain->station->modules[i]->hit))
                    {
                        if(subMain->station->modules[i]->volume)
                        {
                            if((subMain->station->modules[i]->contents == NULL && inHandA->members.size() > subMain->station->modules[i]->capacity) ||
                               (subMain->station->modules[i]->contents != NULL && subMain->station->modules[i]->contents->members.size() + inHandA->members.size() > subMain->station->modules[i]->capacity))
                            {
                                std::string s;
                                s += "Team is to big: Module can hold ";
                                if(subMain->station->modules[i]->contents == NULL)
                                {
                                    s += num_to_string(subMain->station->modules[i]->capacity) + " astronaut";
                                }
                                else
                                {
                                    s += num_to_string(subMain->station->modules[i]->capacity - subMain->station->modules[i]->contents->members.size()) + " more";
                                }
                                create_popups(s);
                                placed = true;//not really but for indoors purposes yes
                            }
                            else
                            {
                                if(subMain->station->modules[i]->contents != NULL)
                                {
                                    create_popups("Teams Combined");
                                    subMain->station->modules[i]->contents->extend(inHandA);
                                }
                                else
                                {
                                    subMain->station->modules[i]->contents = inHandA;
                                    inHandA->inside = subMain->station->modules[i];
                                    inHandA->move_to(subMain->station->modules[i]->tex->texCoords->orig.x,subMain->station->modules[i]->tex->texCoords->orig.y);
                                    for(int j = 0;j < inHandA->members.size();j++)
                                    {
                                        inHandA->members[j]->inside = subMain->station->modules[i];
                                    }
                                }
                                inHandA = NULL;//place astronaut
                                placed = true;
                            }
                        }
                        else
                        {
                            create_popups("Non Habitable Module");
                        }
                        break;
                    }
                }
                if(!placed)
                {
                    create_popups("Please Place Indoors");
                }
            }
        }
    }
}

void SubMain::key_logic()
{
    if(gameMain->keyStates[SDL_SCANCODE_W])
    {
        Point p(0,-1 * windowMoveSpeed * subMain->station->gameSpaceScale);
        p.rotate(subMain->station->gameSpace->rot);
        subMain->station->gameSpace->orig = subMain->station->gameSpace->orig + p;
    }
    if(gameMain->keyStates[SDL_SCANCODE_S])
    {
        Point p(0,windowMoveSpeed * subMain->station->gameSpaceScale);
        p.rotate(subMain->station->gameSpace->rot);
        subMain->station->gameSpace->orig = subMain->station->gameSpace->orig + p;
    }
    if(gameMain->keyStates[SDL_SCANCODE_A])
    {
        Point p(-1 * windowMoveSpeed * subMain->station->gameSpaceScale,0);
        p.rotate(subMain->station->gameSpace->rot);
        subMain->station->gameSpace->orig = subMain->station->gameSpace->orig + p;
    }
    if(gameMain->keyStates[SDL_SCANCODE_D])
    {
        Point p(windowMoveSpeed * subMain->station->gameSpaceScale,0);
        p.rotate(subMain->station->gameSpace->rot);
        subMain->station->gameSpace->orig = subMain->station->gameSpace->orig + p;
    }
    if(gameMain->keyStates[SDL_SCANCODE_Q])
    {
        subMain->station->gameSpace->rotate(windowRotSpeed);
    }
    if(gameMain->keyStates[SDL_SCANCODE_E])
    {
        subMain->station->gameSpace->rotate(-1 * windowRotSpeed);
    }
}

void SubMain::calc_mouse_pos()
{
    gameMousePos.x = (mousePos.x - (camW / 2));// + gameSpace->orig.x;
    gameMousePos.y = (mousePos.y - (camH / 2));// + gameSpace->orig.y;
    gameMousePos.x *= subMain->station->gameSpaceScale;// + gameSpace->orig.x;
    gameMousePos.y *= subMain->station->gameSpaceScale;// + gameSpace->orig.y;
    gameMousePos.rotate(subMain->station->gameSpace->rot);
    gameMousePos.x += subMain->station->gameSpace->orig.x;
    gameMousePos.y += subMain->station->gameSpace->orig.y;
}

void SubMain::update_window()
{
    overlays[DetailType::OXYGEN]->change(0);
    overlays[DetailType::TEMPERATURE]->change(0);
}

void SubMain::show_tool_tip()
{
    if(toolTip.size() > 0)
    {
        std::vector<Text *> texes;
        for(int i = 0;i < toolTip.size();i++)
        {
            texes.push_back(new Text(1000,30,mousePos.x + 10,mousePos.y + 5 + (40 * i),toolTip[i],255,0,0,false));
        }
        int longest = 0;
        int height = 0;
        for(int i = 0;i < texes.size();i++)
        {
            if(texes[i]->texCoords->vert[1].x - texes[i]->texCoords->vert[0].x  > longest)
            {
                longest = texes[i]->texCoords->vert[1].x - texes[i]->texCoords->vert[0].x;
            }
            height += texes[i]->texCoords->vert[2].y - texes[i]->texCoords->vert[1].y;
        }
        height += 30;
        render_rect(mousePos.x - 5,mousePos.y - 5,longest + 30,toolTip.size() * 40 + 10,1,1,1,1);
        render_rect(mousePos.x,mousePos.y,longest + 20,toolTip.size() * 40,0,0,0,1);
        for(int i = 0;i < texes.size();i++)
        {
            texes[i]->render();
            delete texes[i];
        }
        toolTip.clear();
    }
}

void SubMain::create_popups(std::string s)
{
    popups.push_back(s);
    popups_time.push_back(SDL_GetTicks() + s.size() * 100);//default wait time is on second for ten characters
}

void SubMain::show_popups()
{
    Point loc;
    if(subMain->station->stage_type == 1)
    {
        loc = Point(20,250);
    }
    else
    {
        loc = Point(20,400);
    }
    if(popups.size() > 0)
    {
        std::vector<Text *> texes;
        for(int i = 0;i < popups.size();i++)
        {
            texes.push_back(new Text(1000,30,loc.x + 10,loc.y + 5 + (40 * i),popups[i],255,0,0,false));
        }
        int longest = 0;
        for(int i = 0;i < texes.size();i++)
        {
            if(texes[i]->texCoords->vert[1].x - texes[i]->texCoords->vert[0].x  > longest)
            {
                longest = texes[i]->texCoords->vert[1].x - texes[i]->texCoords->vert[0].x;
            }
        }
        render_rect(loc.x - 5,loc.y - 5,longest + 30,popups.size() * 40 + 10,1,1,1,1);
        render_rect(loc.x,loc.y,longest + 20,popups.size() * 40,0,0,0,1);
        for(int i = 0;i < texes.size();i++)
        {
            texes[i]->render();
            delete texes[i];
        }
        for(int i = 0;i < popups_time.size();i++)
        {
            if(SDL_GetTicks() > popups_time[i])
            {
                popups.erase(popups.begin() + i);
                popups_time.erase(popups_time.begin() + i);
            }
        }

    }
}
