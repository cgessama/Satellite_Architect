
#include "SourceModule/modules.h"
#include "SourceGame/subMain.h"
#include "SourceStations/stations.h"
#include "SourceCore/globals.h"
#include "SourceStates/game_state.h"
#include "SourceCore/consts.h"
#include "SourceStations/resource_enums.h"

HeliPadModule::HeliPadModule()
{
    nodes.push_back(new Node(150,0,this));
    nodes.push_back(new Node(0,150,this));
    nodes[1]->rotation = 90;
    nodes.push_back(new Node(-150,0,this));
    nodes[2]->rotation = 180;
    nodes.push_back(new Node(0,-150,this));
    nodes[3]->rotation = 270;
    tex = new Image(-150,-150,300,300,0,"modules/ground/HeliMod.png",0,0);
	heliTex = new Image(-54, -90, 108, 300, 0, "modules/ground/heli.png", 0, 0);
	rotorTex = new Image(-100, -100, 200, 200, 0, "modules/ground/prop.png", 0, 0);
    hit = new HitBox(-149,-149,298,298,0,0,0);
    volume = false;
    timeBetweenDeliveries = 3600;//1 minute
	timeSince = 0;
	heliTex->texCoords->orig = Point(hit->orig.x + heliOffset, hit->orig.y);
	int foo = rand() % 360;
	heliTex->texCoords->orig.rotate(foo);
	heliTex->texCoords->rotate(foo - heliTex->texCoords->rot - 90);
	heliSpeed = float(heliOffset) / timeBetweenDeliveries;
}

void HeliPadModule::render()
{
	base_render();
	heliTex->render();
	rotorTex->render();
}

void HeliPadModule::logic()
{
    base_logic();
	timeSince += gameMain->timeAccel;
	if (timeSince >= timeBetweenDeliveries)
	{
		heliTex->texCoords->orig = Point(hit->orig.x + heliOffset,hit->orig.y);
		int foo = rand() % 360;
		heliTex->texCoords->orig.rotate(foo);
		heliTex->texCoords->rotate(foo - heliTex->texCoords->rot - 90);
		timeSince = 0;
		for (int i = 0; i < subMain->station->resources.size(); i++)
		{
			float temp = (subMain->station->resourcePurchaseMaximum[i] - subMain->station->resources[i]) * subMain->station->stage->resourceCosts[i];
			float money_spent = 0;
			std::string s;
			if (gameMain->money > temp)//there is enough money for a full purchase
			{
				money_spent = temp;
				s = num_to_string(rInt(temp)) + "$ spent on delivery of " + num_to_string(rInt(subMain->station->resourcePurchaseMaximum[i] - subMain->station->resources[i]));
				subMain->station->resources[i] = subMain->station->resourcePurchaseMaximum[i];
			}
			else if (gameMain->money > 0)
			{
				money_spent = gameMain->money;
				s = "Remaining money spent on delivery of " + num_to_string(rInt(money_spent / subMain->station->stage->resourceCosts[i]));
				subMain->station->resources[i] += money_spent / subMain->station->stage->resourceCosts[i];
			}
			if (money_spent > 0)
			{
				if (i == ResourceType::FOOD)
				{
					s += " food";
				}
				else if (i == ResourceType::DUCTTAPE)
				{
					s += " ducttape";
				}
			}
			gameMain->money -= money_spent;
			subMain->create_popups(s);
		}
		
	}
	Point temp(heliSpeed, 0);
	temp.rotate(heliTex->texCoords->rot - 90);
	heliTex->texCoords->orig = heliTex->texCoords->orig + temp * gameMain->timeAccel;
	rotorTex->texCoords->orig = heliTex->texCoords->orig;
	rotorTex->texCoords->rotate(5 * gameMain->timeAccel);
}

void HeliPadModule::mouse_over()
{
	subMain->toolTip.push_back("Delivery in " + num_to_string((timeBetweenDeliveries - timeSince) / FPS) + " seconds");
}
