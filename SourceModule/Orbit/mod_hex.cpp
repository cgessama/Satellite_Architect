#include "SourceModule/modules.h"
#include "SourceGame/subMain.h"
#include "SourceStations/stations.h"

HexModule::HexModule()
{
	capacity = 3;
	Point pos(0, 90);
	for (int i = 0; i < 6; i++)
	{
		nodes.push_back(new Node(pos.x, pos.y, this));
		pos.rotate(60);
		nodes[i]->rotation = i * 60 + 90;
	}
	tex = new Image(-100, -100, 200, 200, 0, "modules/orbit/HexMod.png", 0, 0);
	hit = new HitBox();
	Point temp(0, 96);
	temp.rotate(30);
	for (int i = 0; i < 6; i++)
	{
		hit->vert.push_back(temp);
		temp.rotate(60);
	}
	
	energyProduction = 1;
	energyStorage = 10;
	station->totalEnergyStorage += energyStorage;
}

void HexModule::render()
{
	base_render();
}

void HexModule::logic()
{
	base_logic();
}

