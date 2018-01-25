#include "SourceModule/modules.h"
#include "SourceGame/subMain.h"
#include "SourceStations/stations.h"


LaunchPadModule::LaunchPadModule()
{
    nodes.push_back(new Node(-500,0,this));
    nodes[0]->rotation = 180;
    tex = new Image(-500,-200,1000,400,0,"modules/ground/launchPadMod.png",0,0);
    hit = new HitBox(-499,-199,998,398,0,0,0);
    volume = false;
}

void LaunchPadModule::render()
{
    base_render();
}

void LaunchPadModule::logic()
{
    base_logic();
}

