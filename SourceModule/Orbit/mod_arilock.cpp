#include "SourceModule/modules.h"
#include "SourceGame/subMain.h"

AirlockModule::AirlockModule()
{
    Point pos(12.5,0);
    for( int i = 0;i < 2;i++)
    {
        nodes.push_back(new Node(pos.x,pos.y,this));
        pos.rotate(180);
        nodes[i]->rotation = i * 180;
        //both nodes start with vertical true;
    }
    tex = new Image(-12.5,-50,25,100,0,"modules/Orbit/airlockMod.png",0,0);
    hit = new HitBox(-11.5,-49,23,98,0,0,0);
    volume = false;
}

void AirlockModule::render()
{
    base_render();
}

void AirlockModule::logic()
{
    base_logic();
}
