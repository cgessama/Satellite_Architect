#include "SourceModule/modules.h"
#include "SourceGame/subMain.h"

BatteryModule::BatteryModule()
{
    nodes.push_back(new Node(25,0,this));
    nodes[0]->rotation = 0;
    nodes.push_back(new Node(-25,0,this));
    nodes[1]->rotation = 180;
    tex = new Image(-25,-50,50,100,0,"modules/Orbit/BatteryMod.png",0,0);
    hit = new HitBox(-24,-49,48,98,0,0,0);
    energyStorage = 10000;
    energyStored = 10000;
    volume = false;
}

void BatteryModule::render()
{
    base_render();
}

void BatteryModule::logic()
{
    base_logic();
}
