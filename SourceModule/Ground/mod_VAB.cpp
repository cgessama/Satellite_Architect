#include "SourceModule/modules.h"
#include "SourceGame/subMain.h"
#include "SourceStations/stations.h"

VABModule::VABModule()
{
    nodes.push_back(new Node(400,0,this));
    nodes.push_back(new Node(0,300,this));
    nodes[1]->rotation = 90;
    nodes.push_back(new Node(-400,0,this));
    nodes[2]->rotation = 180;
    nodes.push_back(new Node(0,-300,this));
    nodes[3]->rotation = 270;
    tex = new Image(-400,-300,800,600,0,"modules/ground/VABMod.png",0,0);
    hit = new HitBox(-399,-299,798,598,0,0,0);
    capacity = 20;
}

void VABModule::render()
{
    base_render();
}

void VABModule::logic()
{
    base_logic();
}

