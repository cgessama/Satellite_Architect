
#include "SourceOverlays/Overlays.h"
#include "SourceGame/subMain.h"

Overlay::Overlay()
{
    pickedUp = false;
}

void Overlay::base_render()
{
    bkg->render();
}

void SubMain::toggle_overlay(int x)
{
    int temp = find_overlay(x);
    if(x == OverlayType::EDITOR)
    {
        if(temp != -1)
        {
            if(inHand.size() > 0)
            {
                delete_inHand();
            }
            activeOverlays.erase(activeOverlays.begin() + temp);
        }
        else//there wasn't one open
        {
            add_overlay(OverlayType::EDITOR);
        }
    }
    else if(x == OverlayType::MANAGER)
    {
        if(temp != -1)
        {
            activeOverlays.erase(activeOverlays.begin() + temp);
            if(inHandA == NULL)
            {
                delete inHandA;
            }
        }
        else//there wasn't one open
        {
            add_overlay(OverlayType::MANAGER);
        }
    }
    else if(x == OverlayType::OXYGEN)
    {
        if(temp != -1)
        {
            activeOverlays.erase(activeOverlays.begin() + temp);
        }
        else
        {
            add_overlay(OverlayType::OXYGEN);
        }
    }
    else if(x == OverlayType::TEMPERATURE)
    {
        if(temp != -1)
        {
            activeOverlays.erase(activeOverlays.begin() + temp);
        }
        else
        {
            add_overlay(OverlayType::TEMPERATURE);
        }
    }
    else if(x == OverlayType::INTEGRITY)
    {
        if(temp != -1)
        {
            activeOverlays.erase(activeOverlays.begin() + temp);
        }
        else
        {
            add_overlay(OverlayType::INTEGRITY);
        }
    }
    else if(x == OverlayType::EXIT)
    {
        if(temp != -1)
        {
            activeOverlays.erase(activeOverlays.begin() + temp);
            gameMain->timeAccel = 1;
        }
        else//there wasn't one open
        {
            add_overlay(OverlayType::EXIT);
            gameMain->timeAccel = 0;
        }
    }
    else if(x == OverlayType::ENERGY)
    {
        if(temp != -1)
        {
            activeOverlays.erase(activeOverlays.begin() + temp);
        }
        else//there wasn't one open
        {
            add_overlay(OverlayType::ENERGY);
        }
    }
}

int SubMain::find_overlay(int v)
{
    for(int i = 0;i < activeOverlays.size();i++)
    {
        if(activeOverlays[i]->type == v)
        {
            return i;
        }
    }
    return -1;
}

void SubMain::add_overlay(int v)
{
    activeOverlays.insert(activeOverlays.begin(),overlays[v]);//open top layer
    for(int j = 0;j < overlays[v]->excludes.size();j++)
    {
        if(find_overlay(overlays[v]->excludes[j]) != -1)//if there is an active exclude
        {
            toggle_overlay(overlays[v]->excludes[j]);//close it
        }
    }
}

void SubMain::overlay_to_front(int v)
{
    int temp = find_overlay(v);
    if(temp != -1)
    {
        activeOverlays.erase(activeOverlays.begin() + temp);
    }
    add_overlay(v);
}








































