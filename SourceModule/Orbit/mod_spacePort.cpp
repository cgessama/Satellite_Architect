#include "SourceModule/modules.h"
#include "SourceGame/subMain.h"
#include "SourceStations/stations.h"
#include "SourceAstronauts/team.h"
#include "SourceStates/game_state.h"
#include "SourceCore/consts.h"
#include "SourceStations/resource_enums.h"

SpacePortModule::SpacePortModule()
{
	nodes.push_back(new Node(25, 0, this));
	nodes[0]->rotation = 0;
	tex = new Image(-25, -50, 50, 100, 0, "modules/Orbit/spacePort.png", 0, 0);
	podTex = new Image(-50, -50, 100, 100, 0, "modules/Orbit/pod.png", 0, 0);
	hit = new HitBox(-24, -49, 48, 98, 0, 0, 0);
	volume = false;
	podRespawn = Point(-1000, 0);
	podOffset = Point(-72, 0);//not 75 because the loc has a aysmtote so it will never make it
	podLoc = podRespawn;
}

void SpacePortModule::render()
{
	base_render();
	podTex->render();
}

void SpacePortModule::logic()
{
	base_logic();
	podLoc = podLoc * (1 - (.01 * gameMain->timeAccel));
	timeSince += gameMain->timeAccel;
	if (timeSince > deliveryDelay)
	{
		podLoc = podRespawn;
		timeSince = 0;
		for (int i = 0; i < subMain->station->resources.size(); i++)
		{
			int temp = (subMain->station->resourcePurchaseMaximum[i] - subMain->station->resources[i]) * subMain->station->stage->resourceCosts[i];
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
			if (rInt(money_spent) > 0)
			{
				if (i == ResourceType::FOOD)
				{
					s += " food";
				}
				else if (i == ResourceType::DUCTTAPE)
				{
					s += " ducttape";
				}
				subMain->create_popups(s);
			}
			gameMain->money -= money_spent;
		}
	}
	podTex->texCoords->orig = tex->texCoords->orig + podLoc + podOffset;
}

void SpacePortModule::rotate(float r)
{
	podRespawn.rotate(r);
	podOffset.rotate(r);//these are the only things added

	tex->texCoords->rotate(r);
	hit->rotate(r);
	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i]->tex->texCoords->orig.rotate(r);
	}
	if (contents != NULL)
	{
		contents->rotate(r);
	}
}

void SpacePortModule::mouse_over()
{
	subMain->toolTip.push_back("Delivery in " + num_to_string(int(float(deliveryDelay - timeSince) / FPS)) + " seconds");
}