#include "SourceStations/stations.h"
#include "SourceGame/subMain.h"
#include "SourceModule/modules.h"
#include "SourceStates/game_state.h"
#include "SourceStations/resource_enums.h"
#include "SourceCore/consts.h"

Station::Station(int stg)
{
    energyProduced = 0;
    energyConsumed = 0;
    energyStored = 0;
    totalEnergyStorage = 0;
    gameSpace = new HitBox(-960,-540,1920,1080,0,0,0);
    gameSpaceScale = 1;
    if(stg == 0)
    {
        stage = sTGOrbit;
    }
    else
    {
        stage = sTGGround;
    }
    stage_type = stg;
}

void Station::logic()
{
    for(int i = 0;i < modules.size();i++)
    {
        modules[i]->logic();
    }
    if(gameMain->timeAccel != 0)
    {
        energyStored = .01;
        totalEnergyStorage = 0;
        energyProduced = 0;
        energyConsumed = 0;
        energyRequested = 0.01;//nan prevention
        for(int i = 0;i < modules.size();i++)
        {
            modules[i]->get_energy();
        }
        energyFactor = (energyProduced + energyStored) / (energyRequested * gameMain->timeAccel);
        if(energyFactor > 1)
        {
            energyFactor = 1;
        }
        energyStorageFactor = 1 + ((((-energyRequested * energyFactor) + energyProduced * gameMain->timeAccel) / energyStored) / FPS);
        for(int i = 0;i < modules.size();i++)
        {
            modules[i]->send_energy();
        }
    }
}

Station::~Station()
{
}


























