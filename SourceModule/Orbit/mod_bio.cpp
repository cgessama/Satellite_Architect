#include "SourceModule/modules.h"
#include "SourceGame/subMain.h"
#include "SourceStations/stations.h"

BioModule::BioModule()
{
	capacity = 3;
	nodes.push_back(new Node(-50, -100, this));
	nodes[nodes.size() - 1]->rotation = 270;
	nodes.push_back(new Node( 50, -100, this));
	nodes[nodes.size() - 1]->rotation = 270;
	nodes.push_back(new Node(100, 50, this));
	nodes[nodes.size() - 1]->rotation = 0;
	nodes.push_back(new Node(100, -50, this));
	nodes[nodes.size() - 1]->rotation = 0;
	nodes.push_back(new Node( 50,  100, this));
	nodes[nodes.size() - 1]->rotation = 90;
	nodes.push_back(new Node(-50,  100, this));
	nodes[nodes.size() - 1]->rotation = 90;
	nodes.push_back(new Node(-100, -50, this));
	nodes[nodes.size() - 1]->rotation = 180;
	nodes.push_back(new Node(-100,  50, this));
	nodes[nodes.size() - 1]->rotation = 180;
    tex = new Image(-100,-100,200,200,0,"modules/orbit/bioMod.png",0,0);
    hit = new HitBox(-99,-99, 198, 198,0,0,0);
    energyProduction = 1;
    energyStorage = 10;
    station->totalEnergyStorage += energyStorage;
}

void BioModule::render()
{
    base_render();
}

void BioModule::logic()
{
    base_logic();
}

