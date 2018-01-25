#include "SourceAstronauts/astronaut.h"
#include "SourceModule/modules.h"
#include "SourceGame/subMain.h"
#include "SourceStates/game_state.h"//money
#include "SourceAstronauts/team.h"
#include "SourceResearch/Option.h"
#include "SourceStations/resource_enums.h"
#include "SourceModule/detail_enums.h"
#include "SourceCore/consts.h"


Astronaut::Astronaut()
{
    tex = new Image(-20,-40,40,80,0,"astronaut/main.png",mousePos.x,mousePos.y);
    inside = NULL;
}

void Astronaut::logic()
{
    overlays.clear();
    if(inside != NULL)
    {
        if(inside->details[DetailType::OXYGEN]->value < 25)//25 % oxy
        {
            overlays.push_back(create_pic("astronaut/over_no_oxy.png"));
        }
        if(inside->details[DetailType::TEMPERATURE]->value < 10)
        {
            overlays.push_back(create_pic("astronaut/over_ice.png"));
        }
        if(inside->details[DetailType::TEMPERATURE]->value > 25)
        {
            overlays.push_back(create_pic("astronaut/over_fire.png"));
        }
    }
    else
    {
        if(subMain->station->stage->default_oxy < 25)//25 % oxy
        {
            overlays.push_back(create_pic("astronaut/over_no_oxy.png"));
        }
        if(subMain->station->stage->default_temp < 10)
        {
            overlays.push_back(create_pic("astronaut/over_ice.png"));
        }
        if(subMain->station->stage->default_temp > 25)
        {
            overlays.push_back(create_pic("astronaut/over_fire.png"));
        }
        overlays.push_back(create_pic("astronaut/over_no_food.png"));
    }
    if(overlays.size() > 0)
    {
        overlays.insert(overlays.begin(),create_pic("astronaut/over_sad.png"));
    }
    else//astronaut is happy
    {
        if(inside != NULL)
        {
            gameMain->money += inside->income * gameMain->timeAccel;
            if(subResearch->queue.size() != 0)
            {
                subResearch->queue[0]->completion += inside->income * gameMain->timeAccel;
            }
        }
    }
}

void Astronaut::render()
{
    tex->render();
    if(overlays.size() > 0)
    {
        Image temp;
        temp.texCoords = new HitBox(*tex->texCoords);
        for(int i = 0;i < overlays.size();i++)
        {
            temp.texData = overlays[i];
            temp.render();
        }
    }
}

Astronaut::~Astronaut()
{
    delete tex;
    if(inside != NULL)
    {
        inside->contents = NULL;
    }
    inside = NULL;
    overlays.clear();
    for(int i = 0;i < memberOf->members.size();i++)
    {
        if(memberOf->members[i] == this)
        {
            memberOf->members.erase(memberOf->members.begin() + i);
        }
    }
}





