#include "SourceOverlays/Overlays.h"
#include "SourceModule/modules.h"
#include "SourceStates/game_state.h"
#include "SourceResearch/Option.h"
#include "SourceCore/colide.h"

OverlayEditor::OverlayEditor()
{
    bkg = new Image(-camW / 2,-camH / 2,camW,camH,0,"Overlays/Editor.png",camW / 2,camH / 2);
    name = new Text(1000,80,1550,990,"Editor",0,0,0,false);
    locking = true;
    lockingTex = new Text(500,25,1675,950,"Locking: True",255,0,0,false);
    type = OverlayType::EDITOR;
    excludes.push_back(OverlayType::MANAGER);//no astro editor
    init_stage0();
    init_stage1();
    for(int i = 0;i < Buttons[0].size();i++)
    {
        hits.push_back(Buttons[0][i]->texCoords);
    }
}

void OverlayEditor::render()
{
    base_render();
    for(int i = 0;i < Buttons[subMain->station->stage_type].size();i++)
    {
        if(techLevels[subMain->station->stage_type][i]->completion >= techLevels[subMain->station->stage_type][i]->cost)
        {
            Buttons[subMain->station->stage_type][i]->render();
        }
    }
    name->render();
    lockingTex->render();
}

float OverlayEditor::get_val(int v)
{
    if(v == 0)
    {
        return locking;
    }
    return 0;
}

void OverlayEditor::change(int x)
{
	if (x > Buttons[0].size() - 1)
	{
		subMain->create_popups("Module Refunded for " + num_to_string(prices[1][x - Buttons[0].size()]) + "$");
	}
	else
	{
		subMain->create_popups("Module Refunded for " + num_to_string(prices[0][x]) + "$");
	}
}

bool OverlayEditor::handle_click()
{
    bool temp = false;
    for(int i = 0;i < hits.size();i++)
    {
        if(lp_point(mousePos,*hits[i]))
        {
            temp = true;
            break;
        }
    }
    if(temp)//click was on the UI
    {
        if(subMain->inHand.size() == 0)
        {
            for(int i = 0;i < Buttons[subMain->station->stage_type].size();i++)
            {
                if(techLevels[subMain->station->stage_type][i]->completion >= techLevels[subMain->station->stage_type][i]->cost &&
                   lp_point(mousePos,*Buttons[subMain->station->stage_type][i]->texCoords))
                {
                    if(gameMain->money < prices[subMain->station->stage_type][i])
                    {
                        subMain->create_popups("You need " + num_to_string(prices[subMain->station->stage_type][i] - gameMain->money) + " more Dollars");
                    }
                    else
                    {
                        int index = 0;
                        for(int j = 0;j < subMain->station->stage_type;j++)
                        {
                            index += Buttons[j].size();
                        }
                        new_module(index + i);
                        gameMain->money -= prices[subMain->station->stage_type][i];
                    }
                    break;
                }
            }
        }
        return false;//do not continue
    }
    return true;
}

bool OverlayEditor::handle_key()
{
    bool r = true;//no action
    if(!r)//the key was bound
    {
        if(event.key.keysym.sym == 108)//l
        {
            locking = !locking;
            if(locking)
            {
                lockingTex->change_text("Locking: True");
            }
            else
            {
                lockingTex->change_text("Locking: False");
            }
            r = false;
        }
    }
    return r;
}

void OverlayEditor::logic()
{
    for(int i = 0;i < Buttons[subMain->station->stage_type].size();i++)
    {
        if(techLevels[subMain->station->stage_type][i]->completion >= techLevels[subMain->station->stage_type][i]->cost &&
           lp_point(mousePos,*Buttons[subMain->station->stage_type][i]->texCoords))
        {
            subMain->toolTip.push_back("Price:" + num_to_string(prices[subMain->station->stage_type][i]));
        }
    }
    subMain->inhand_logic();
}


void OverlayEditor::init_stage0()
{
    std::vector<int> p;
    std::vector<TechOption *> t;
    std::vector<Texture *> b;
    prices.push_back(p);
    techLevels.push_back(t);
    Buttons.push_back(b);

    prices[0].push_back(100);
    prices[0].push_back(500);
    prices[0].push_back(200);
    prices[0].push_back(500);
	prices[0].push_back(300);
	prices[0].push_back(500);

    prices[0].push_back(1000);
    prices[0].push_back(100);
    prices[0].push_back(3000);
    prices[0].push_back(5000);

    techLevels[0].push_back(subResearch->techOptions[0]);
	techLevels[0].push_back(subResearch->techOptions[0]);
    techLevels[0].push_back(subResearch->techOptions[0]);
    techLevels[0].push_back(subResearch->techOptions[0]);
	techLevels[0].push_back(subResearch->techOptions[0]);
	techLevels[0].push_back(subResearch->techOptions[0]);

    techLevels[0].push_back(subResearch->techOptions[3]);
    techLevels[0].push_back(subResearch->techOptions[1]);
    techLevels[0].push_back(subResearch->techOptions[4]);
    techLevels[0].push_back(subResearch->techOptions[2]);

	Buttons[0].push_back(new Image(-buttonDim / 2, -buttonDim / 2, buttonDim    , buttonDim, 0, "modules/Orbit/basicMod.png"     , buttonOrigOffX + buttonOffX * 0, camH - buttonOffY));
	Buttons[0].push_back(new Image(-buttonDim / 4, -buttonDim / 2, buttonDim / 2, buttonDim, 0, "modules/Orbit/HeaterMod.png"    , buttonOrigOffX + buttonOffX * 1, camH - buttonOffY));
	Buttons[0].push_back(new Image(-buttonDim / 4, -buttonDim / 2, buttonDim / 2, buttonDim, 0, "modules/Orbit/OxygenTankMod.png", buttonOrigOffX + buttonOffX * 2, camH - buttonOffY));
	Buttons[0].push_back(new Image(-buttonDim / 4, -buttonDim / 2, buttonDim / 2, buttonDim, 0, "modules/Orbit/BatteryMod.png"   , buttonOrigOffX + buttonOffX * 3, camH - buttonOffY));
	Buttons[0].push_back(new Image(-buttonDim / 2, -buttonDim / 2, buttonDim    , buttonDim, 0, "modules/Orbit/HexMod.png", buttonOrigOffX + buttonOffX * 4, camH - buttonOffY));
	Buttons[0].push_back(new Image(-buttonDim / 2, -buttonDim / 2, buttonDim,     buttonDim, 0, "modules/Orbit/BioMod.png", buttonOrigOffX + buttonOffX * 5, camH - buttonOffY));

	
	Buttons[0].push_back(new Image(-buttonDim / 2, -buttonDim / 2, buttonDim    , buttonDim, 0, "modules/Orbit/advMod.png", buttonOrigOffX + buttonOffX * 6, camH - buttonOffY));
	Buttons[0].push_back(new Image(-buttonDim / 8, -buttonDim / 2, buttonDim / 4, buttonDim, 0, "modules/Orbit/airlockMod.png", buttonOrigOffX + buttonOffX * 7, camH - buttonOffY));
	Buttons[0].push_back(new Image(-buttonDim / 4, -buttonDim / 2, buttonDim / 2, buttonDim, 0, "modules/Orbit/OxygenSynthMod.png", buttonOrigOffX + buttonOffX * 8, camH - buttonOffY));
	Buttons[0].push_back(new Image(-buttonDim / 6, -buttonDim / 2, buttonDim / 3, buttonDim, 0, "modules/Orbit/SolarPanelMod.png", buttonOrigOffX + buttonOffX * 9, camH - buttonOffY));//these must line up with enums
}

void OverlayEditor::init_stage1()
{
    std::vector<int> p;
    std::vector<TechOption *> t;
    std::vector<Texture *> b;
    prices.push_back(p);
    techLevels.push_back(t);
    Buttons.push_back(b);

    prices[1].push_back(100);
    prices[1].push_back(100);
	prices[1].push_back(100);
	prices[1].push_back(100);
	prices[1].push_back(100);

    techLevels[1].push_back(subResearch->techOptions[0]);
    techLevels[1].push_back(subResearch->techOptions[0]);
	techLevels[1].push_back(subResearch->techOptions[0]);
	techLevels[1].push_back(subResearch->techOptions[0]);
	techLevels[1].push_back(subResearch->techOptions[0]);

    Buttons[1].push_back(new Image(-buttonDim / 2,-buttonDim / (2.0*(4.0/3.0)),buttonDim,buttonDim / (4.0/3.0),0,"modules/Ground/VABMod.png"      ,buttonOrigOffX + buttonOffX * 0,camH - buttonOffY));
    Buttons[1].push_back(new Image(-buttonDim / 2,-buttonDim / ((5.0/2.0)*2.0),buttonDim,buttonDim / (5.0/2.0),0,"modules/Ground/launchPadMod.png",buttonOrigOffX + buttonOffX * 1,camH - buttonOffY));
	Buttons[1].push_back(new Image(-buttonDim / 2, -buttonDim / 2, buttonDim, buttonDim, 0, "modules/Ground/RandDMod.png", buttonOrigOffX + buttonOffX * 2, camH - buttonOffY));//these must line up with ModuleType
	Buttons[1].push_back(new Image(-buttonDim / 2, -buttonDim / 2, buttonDim, buttonDim, 0, "modules/Ground/coms.png", buttonOrigOffX + buttonOffX * 3, camH - buttonOffY));//these must line up with ModuleType
}































