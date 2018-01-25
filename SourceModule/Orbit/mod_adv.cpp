#include "SourceModule/modules.h"
#include "SourceGame/subMain.h"

AdvancedModule::AdvancedModule()
{
    Point pos(50,0);
    for( int i = 0;i< 4;i++)
    {
        nodes.push_back(new Node(pos.x,pos.y,this));
        pos.rotate(90);
        nodes[i]->rotation = i * 90;
    }
    tex = new Image(-50,-50,100,100,0,"modules/Orbit/advMod.png",0,0);
    hit = new HitBox(-49,-49,98,98,0,0,0);
    energyProduction = 20;
    energyStorage = 400;
}

void AdvancedModule::render()
{
    base_render();
}

void AdvancedModule::logic()
{
    base_logic();
}
