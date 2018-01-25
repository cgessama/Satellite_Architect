#include "SourceOverlays/Overlays.h"
#include "SourceStations/stations.h"
#include "SourceCore/colide.h"

OverlayEnergyInfo::OverlayEnergyInfo()
{
    loc = Point(500,500);
    bkg = new Image(-.1,-.1,400,350,0,"Overlays/energyInfo.png",loc.x,loc.y);
    nameTex = new Text(1000,40,loc.x + 40,loc.y + 30,"Energy",0,0,0,false);
    type = OverlayType::ENERGY;
    hits.push_back(new HitBox);
    hits[0]->orig = Point(loc.x + 20,loc.y + 60);
    hits[0]->vert.push_back(Point(-.1,-.1));
    hits[0]->vert.push_back(Point(365,-.1));
    hits[0]->vert.push_back(Point(365,240));
    hits[0]->vert.push_back(Point(330,270));
    hits[0]->vert.push_back(Point(-.1,270));
    hits[0]->fix();
    hits.push_back(new HitBox);
    hits[1]->orig = Point(loc.x + 50,loc.y + 50);
    hits[1]->vert.push_back(Point(-10,-30));
    hits[1]->vert.push_back(Point(140,-30));
    hits[1]->vert.push_back(Point(180,20));
    hits[1]->vert.push_back(Point(-25,20));
    hits[1]->vert.push_back(Point(-25,-5));
    hits[1]->fix();
    pickedUp = false;
    storeTex   = new Text(330,35,loc.x + 40,loc.y + 115,"Stored:"   ,0,0,0,false);
    produceTex = new Text(330,35,loc.x + 40,loc.y + 150,"Produced:" ,0,255,0,false);
    requestTex = new Text(330,35,loc.x + 40,loc.y + 185,"Requested:",255,0,0,false);
    consumeTex = new Text(330,35,loc.x + 40,loc.y + 220,"Consumed:" ,255,0,0,false);
    deficitTex = new Text(330,35,loc.x + 40,loc.y + 255,"Deficit:"  ,255,0,0,false);
    netTex  = new Text(330,35,loc.x + 40,loc.y + 290,"Net:"      ,0,255,0,false);

    storeNumTex = new Text(200,35,loc.x + 275,loc.y + 115 + (35 / 2),"-1",0,0,0,true);
    produceNumTex = new Text(130,35,loc.x + 250,loc.y + 150,"-1",0,255,0,false);
    requestNumTex = new Text(130,35,loc.x + 250,loc.y + 185,"-1",255,0,0,false);
    consumeNumTex = new Text(130,35,loc.x + 250,loc.y + 220,"-1",255,0,0,false);
    deficitNumTex = new Text(130,35,loc.x + 250,loc.y + 255,"-1",255,0,0,false);
    netNumTex = new Text(130,35,loc.x + 250,loc.y + 290,"-1",0,255,0,false);

    oldStored = -1;
    oldStorage = -1;
    oldProduce = -1;
    oldRequest = -1;
    oldConsume = -1;
    oldDeficit = -1;
    oldNet = -1;
}

void OverlayEnergyInfo::render()
{
    update_texes();
    bkg->render();
    nameTex->render();

    storeTex->render();
    produceTex->render();
    consumeTex->render();
    deficitTex->render();
    requestTex->render();
    netTex->render();

    if(subMain->station != NULL)
    {
        storeNumTex->render();
        produceNumTex->render();
        requestNumTex->render();
        consumeNumTex->render();
        deficitNumTex->render();
        netNumTex->render();
    }
}

bool OverlayEnergyInfo::handle_click()
{
    if(lp_point(mousePos,*hits[0])|| lp_point(mousePos,*hits[1]))
    {
        if(!pickedUp)
        {
            if(subMain->inHand.size() == 0 && subMain->inHandA == NULL)
            {
                offset = bkg->texCoords->orig - mousePos;
                pickedUp = true;
            }
        }
        else//object was inHand
        {
            pickedUp = false;
        }
        return false;//do not continue
    }
    return true;
}

void OverlayEnergyInfo::logic()
{
    if(pickedUp)
    {
        Point temp = mousePos - bkg->texCoords->orig + offset;
        move(temp.x,temp.y);
    }
}

void OverlayEnergyInfo::move(float x,float y)
{
    loc = Point(loc.x + x,loc.y + y);
    nameTex->texCoords->orig = Point(loc.x + 40,loc.y + 30);
    bkg->texCoords->orig = loc;

    storeTex  ->texCoords->orig = Point(loc.x + 40,loc.y + 115);
    produceTex->texCoords->orig = Point(loc.x + 40,loc.y + 150);
    requestTex->texCoords->orig = Point(loc.x + 40,loc.y + 185);
    consumeTex->texCoords->orig = Point(loc.x + 40,loc.y + 220);
    deficitTex->texCoords->orig = Point(loc.x + 40,loc.y + 255);
    netTex    ->texCoords->orig = Point(loc.x + 40,loc.y + 290);

    storeNumTex  ->texCoords->orig = Point(loc.x + 275,loc.y + 115 + (35 / 2));
    produceNumTex->texCoords->orig = Point(loc.x + 250,loc.y + 150);
    requestNumTex->texCoords->orig = Point(loc.x + 250,loc.y + 185);
    consumeNumTex->texCoords->orig = Point(loc.x + 250,loc.y + 220);
    deficitNumTex->texCoords->orig = Point(loc.x + 250,loc.y + 255);
    netNumTex    ->texCoords->orig = Point(loc.x + 250,loc.y + 290);

    hits[0]->orig = Point(loc.x + 20,loc.y + 60);
    hits[1]->orig = Point(loc.x + 50,loc.y + 50);
}

void OverlayEnergyInfo::update_texes()
{
    if(subMain->station != NULL)
    {
        if(oldStored != int(subMain->station->energyStored) || oldStorage != int(subMain->station->totalEnergyStorage))
        {
            oldStored = int(subMain->station->energyStored);
            oldStorage = int(subMain->station->totalEnergyStorage);
            storeNumTex->change_text(num_to_string(int(subMain->station->energyStored)) + " / " +
                                     num_to_string(int(subMain->station->totalEnergyStorage)));
        }

        if(oldProduce != int(subMain->station->energyProduced))
        {
            oldProduce = int(subMain->station->energyProduced);
            produceNumTex->change_text(num_to_string(int(subMain->station->energyProduced)));
        }

        if(oldRequest != int(subMain->station->energyRequested))
        {
            oldRequest = int(subMain->station->energyRequested);
            requestNumTex->change_text(num_to_string(int(subMain->station->energyRequested)));
        }

        if(oldConsume != int(subMain->station->energyConsumed))
        {
            oldConsume = int(subMain->station->energyConsumed);
            consumeNumTex->change_text(num_to_string(int(subMain->station->energyConsumed)));
        }

        int defTemp = int(subMain->station->energyRequested - subMain->station->energyConsumed);
        if(oldDeficit != defTemp)
        {
            oldDeficit = defTemp;
            deficitNumTex->change_text(num_to_string(defTemp));
        }

        int netTemp = int(subMain->station->energyProduced - subMain->station->energyConsumed);
        if(oldNet != netTemp)
        {
            oldNet = netTemp;
            if(netTemp >= 0)
            {
                netNumTex->color.r = 0;
                netNumTex->color.g = 255;
                netTex->color.r = 0;
                netTex->color.g = 255;
            }
            else
            {
                netNumTex->color.r = 255;
                netNumTex->color.g = 0;
                netTex->color.r = 255;
                netTex->color.g = 0;
            }
            netNumTex->change_text(num_to_string(netTemp));
        }
    }
}






















