#include "SourceModule/modules.h"
#include "SourceGame/subMain.h"
#include "SourceStations/stations.h"

RandDModule::RandDModule()
{
    nodes.push_back(new Node(150,0,this));
    nodes.push_back(new Node(0,150,this));
    nodes[1]->rotation = 90;
    nodes.push_back(new Node(-150,0,this));
    nodes[2]->rotation = 180;
    nodes.push_back(new Node(0,-150,this));
    nodes[3]->rotation = 270;
    tex = new Image(-150,-150,300,300,0,"modules/ground/RandDMod.png",0,0);
    hit = new HitBox(-149,-149,298,298,0,0,0);
    capacity = 10;
}

void RandDModule::render()
{
    base_render();
}

void RandDModule::logic()
{
    base_logic();
}


