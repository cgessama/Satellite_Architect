#include "SourceModule/modules.h"
#include "SourceGame/subMain.h"
#include "SourceStations/stations.h"
#include "SourceModule/detail_enums.h"

OxygenTankModule::OxygenTankModule()
{
    nodes.push_back(new Node(25,0,this));
    nodes[0]->rotation = 0;
    tex = new Image(-25,-50,50,100,0,"modules/Orbit/OxygenTankMod.png",0,0);
    hit = new HitBox(-24,-49,48,98,0,0,0);
    energyConsumtion = 10;
    storedOxy = start_level;
    volume = false;
}

void OxygenTankModule::render()
{
    base_render();
}

void OxygenTankModule::get_energy()
{
    if(storedOxy > 0 && nodes[0]->adjacent != NULL && nodes[0]->adjacent->volume)//Producing for a valid chamber
    {
        float ECtemp = 0;
        if(nodes[0]->adjacent->details[DetailType::OXYGEN]->value > 100.0)
        {
            ECtemp = 0;
        }
        else if(100.0 - nodes[0]->adjacent->details[DetailType::OXYGEN]->value < flowRate)//the tank doesn't need to give 100%
        {
            ECtemp = ((100.0 - nodes[0]->adjacent->details[DetailType::OXYGEN]->value) / flowRate) * energyConsumtion;
        }
        else
        {
            ECtemp = energyConsumtion;
        }
        //ajust for the amount of air in tank
        if((ECtemp / energyConsumtion) * flowRate > storedOxy)//there is not enough oxygen in the tank for the requested consumtion
        {
            ECtemp = (ECtemp / energyConsumtion) * flowRate;
        }
        station->energyRequested += ECtemp;
        energyRequested = ECtemp;
    }
    else
    {
        energyRequested = 0;
    }
}

void OxygenTankModule::send_energy()
{
    if(energyRequested > 0 && nodes[0]->adjacent != NULL && nodes[0]->adjacent->volume)//Producing for a valid chamber
    {
        float oxyLoss = flowRate * ((energyRequested * station->energyFactor) / energyConsumtion);
        station->energyConsumed += energyRequested * station->energyFactor;
        nodes[0]->adjacent->details[DetailType::OXYGEN]->value += oxyLoss;
        storedOxy -= oxyLoss;
    }
}

void OxygenTankModule::logic()
{
    base_logic();
}

void OxygenTankModule::mouse_over()
{
    subMain->toolTip.push_back("O2 Remaining: ");
    subMain->toolTip.push_back(num_to_string(int(storedOxy)) + " / " + num_to_string(start_level));
}

