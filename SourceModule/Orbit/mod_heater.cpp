#include "SourceModule/modules.h"
#include "SourceGame/subMain.h"
#include "SourceStations/stations.h"
#include "SourceModule/detail_enums.h"

HeaterModule::HeaterModule()
{
    nodes.push_back(new Node(25,0,this));
    nodes[0]->rotation = 0;
    tex = new Image(-25,-50,50,100,0,"modules/Orbit/HeaterMod.png",0,0);
    hit = new HitBox(-24,-49,48,98,0,0,0);
    energyConsumtion = 500;
    volume = false;
}

void HeaterModule::render()
{
    base_render();
}

void HeaterModule::logic()
{
    base_logic();
}

void HeaterModule::get_energy()
{
    if(nodes[0]->adjacent != NULL && nodes[0]->adjacent->volume)//Producing for a valid chamber
    {
        float ECtemp = 0;
        if(nodes[0]->adjacent->details[DetailType::TEMPERATURE]->value > 20)
        {
            ECtemp = 0;
        }
        else if(20 - nodes[0]->adjacent->details[DetailType::TEMPERATURE]->value < heatRate)//the tank doesn't need to give 100%
        {
            ECtemp = ((20 - nodes[0]->adjacent->details[DetailType::TEMPERATURE]->value) / heatRate) * energyConsumtion;
        }
        else
        {
            ECtemp = energyConsumtion;
        }
        station->energyRequested += ECtemp;
        energyRequested = ECtemp;
    }
    else
    {
        energyRequested = 0;
    }
}

void HeaterModule::send_energy()
{
    if(energyRequested > 0 && nodes[0]->adjacent != NULL && nodes[0]->adjacent->volume)//Producing for a valid chamber
    {
        station->energyConsumed += energyRequested * station->energyFactor;
        nodes[0]->adjacent->details[DetailType::TEMPERATURE]->value += heatRate * ((energyRequested * station->energyFactor) / energyConsumtion);
    }
}


