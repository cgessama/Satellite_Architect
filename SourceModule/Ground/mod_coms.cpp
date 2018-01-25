#include "SourceModule/modules.h"
#include "SourceGame/subMain.h"
#include "SourceStations/stations.h"

ComsModule::ComsModule()
{
	nodes.push_back(new Node(150, 0, this));
	nodes.push_back(new Node(0, 150, this));
	nodes[1]->rotation = 90;
	nodes.push_back(new Node(-150, 0, this));
	nodes[2]->rotation = 180;
	nodes.push_back(new Node(0, -150, this));
	nodes[3]->rotation = 270;
	tex = new Image(-150, -150, 300, 300, 0, "modules/ground/coms.png", 0, 0);
	hit = new HitBox(-149, -149, 298, 298, 0, 0, 0);
	capacity = 10;
}

void ComsModule::render()
{
	base_render();
}

void ComsModule::logic()
{
	base_logic();
}
