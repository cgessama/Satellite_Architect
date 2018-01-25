#include "SourceModule/modules.h"
#include "SourceGame/subMain.h"

SolarPanelModule::SolarPanelModule()
{
    nodes.push_back(new Node(0,150,this));
    nodes[0]->rotation = 90;
    tex = new Image(-50,-150,100,300,0,"modules/Orbit/solarpanelmod.png",0,0);
    hit = new HitBox(-49,-149,98,298,0,0,0);
    energyProduction = 500;
    volume = false;
}

void SolarPanelModule::render()
{
    base_render();
}

void SolarPanelModule::logic()
{
    base_logic();
}
