#include "SourceOverlays/Overlays.h"
#include "SourceStates/game_state.h"
#include "SourceResearch/Option.h"
#include "SourceCore/colide.h"

OverlayInfoBanner::OverlayInfoBanner()
{
    bkg = new Image(-.1,-.1,camW,camH,0,"Overlays/infobanner/infoBanner.png",0,0);
    leaf = new Image(-.1,-.1,300,50,0,"Overlays/infobanner/leaf.png",0,100);
    end_leaf = new Image(-.1,-.1,300,90,0,"Overlays/infobanner/end_leaf.png",0,100);
    type = OverlayType::INFOBANNER;
    moneyTex =    new Text(1000,statsTextH,moneyBeginX   ,statsBeginY,"Money:",0,0,0,false);
    techTex =    new Text(1000,statsTextH,techBeginX   ,statsBeginY,"Tech:",0,0,0,false);
    moneyNumTex = new Text(1000,statsTextH,moneyNumBeginX,statsBeginY,"0",0,0,0,false);
    TechNumTex  = new Text(1000,statsTextH,techNumBeginX,statsBeginY,"0",0,0,0,false);
    playTex = new Image(-.1,-.1,timeHitsDim,timeHitsDim,0,"Overlays/infobanner/play.png",timeHitsXBegin,timeHitsYBegin);
    pauseTex = new Image(-.1,-.1,timeHitsDim,timeHitsDim,0,"Overlays/infobanner/pause.png",pauseHitXBegin,timeHitsYBegin);
    rTex = new Text(100,60,1773,160,"(R)",0,0,0,true);
    mTex = new Text(100,60,1585,125,"(M)",0,0,0,true);

    std::vector<Text *> t;
    buttons.push_back(t);
    buttons.push_back(t);
    buttons[0].push_back(new Text(1000,40,10,10, "Editor"     ,255,0,0,false));
    buttons[0].push_back(new Text(1000,40,10,60, "Management" ,255,0,0,false));
    buttons[0].push_back(new Text(1000,40,10,110,"Energy"     ,255,0,0,false));
    buttons[0].push_back(new Text(1000,40,10,160,"Integrity"  ,255,0,0,false));
    buttons[0].push_back(new Text(1000,40,10,210,"Oxygen"     ,255,0,0,false));
    buttons[0].push_back(new Text(1000,40,10,260,"Temperature",255,0,0,false));


    buttons[1].push_back(new Text(1000,40,10,10, "Editor"     ,255,0,0,false));
    buttons[1].push_back(new Text(1000,40,10,60, "Management" ,255,0,0,false));
    buttons[1].push_back(new Text(1000,40,10,110,"Energy"     ,255,0,0,false));

    hits.push_back(new HitBox(-.1,-.1,1400,60,0,0,0));
    hits[0]->vert[1] = Point(1470,0);
    hits.push_back(new HitBox());
    hits[1]->vert.push_back(Point(-.1,-.1));
    hits[1]->vert.push_back(Point(280,-.1));
    hits[1]->vert.push_back(Point(280,330));
    hits[1]->vert.push_back(Point(240,370));
    hits[1]->vert.push_back(Point(-.1,370));
    hits[1]->fix();
    hits.push_back(new HitBox(-20,-20,40,40,1773,64,0));
    hits.push_back(new HitBox(-20,-58,40,116,1773,139,0));
    hits[3]->vert[2].x += 50;
    hits[3]->vert[3].x -= 50;

    for(int i = 0;i < 4;i++)
    {
        timeHits.push_back(new HitBox(-.1,-.1,timeHitsDim,timeHitsDim,timeHitsXBegin + timeHitsXOffset * i,timeHitsYBegin,0));
    }
}

void OverlayInfoBanner::logic()
{

}

bool OverlayInfoBanner::handle_click()
{
    if(lp_point(mousePos,*hits[2]) || lp_point(mousePos,*hits[3]))
    {
        gameMain->subActive = subResearch;
        return false;
    }
    if(lp_point(mousePos,*mTex->texCoords))
    {
        gameMain->subActive = subMission;
        return false;
    }
    for(int i = 0;i < buttons[subMain->station->stage_type].size();i++)
    {
        if(lp_point(mousePos,*buttons[subMain->station->stage_type][i]->texCoords))
        {
            switch(i){
        case 0:
            subMain->toggle_overlay(OverlayType::EDITOR);
            break;
        case 1:
            subMain->toggle_overlay(OverlayType::MANAGER);
            break;
        case 2:
            subMain->toggle_overlay(OverlayType::ENERGY);
            break;
        case 3:
            subMain->toggle_overlay(OverlayType::INTEGRITY);
            break;
        case 4:
            subMain->toggle_overlay(OverlayType::OXYGEN);
            break;
        case 5:
            subMain->toggle_overlay(OverlayType::TEMPERATURE);
            break;
        default:
            return false;
            break;}
        }
    }
    for(int i = 0;i < timeHits.size();i++)
    {
        if(lp_point(mousePos,*timeHits[i]))
        {
            if(subResearch->queue.size() > 0)
            {
                gameMain->timeAccel = i + 1;
            }
            else
            {
                subMain->create_popups("No Research Selected");
            }
            return false;
            break;
        }
    }
    if(lp_point(mousePos,*pauseTex->texCoords))
    {
		gameMain->timeAccel = 0;
        return false;
    }
    if(lp_point(mousePos,*hits[0]) || lp_point(mousePos,*hits[1]))
    {
        return false;
    }
    return true;
}

void OverlayInfoBanner::render()
{
    bkg->render();
    for(int i = 0;i < buttons[subMain->station->stage_type].size() - 3;i++)
    {
        leaf->render();
        leaf->texCoords->orig.y += 50;
    }
    leaf->texCoords->orig.y = 100;
    end_leaf->texCoords->orig.y = 50 * (buttons[subMain->station->stage_type].size() - 1);
    end_leaf->render();
    std::string s;
    s = num_to_string(int(gameMain->money));
    if(s != moneyNumTex->text)
    {
        moneyNumTex->change_text(s);
    }
    if(subResearch->queue.size() > 0)
    {
        s = num_to_string(int(subResearch->queue[0]->completion)) + "/" + num_to_string(int(subResearch->queue[0]->cost));
    }
    else
    {
        s = "No Research";
    }
    if(int(s != TechNumTex->text))
    {
        TechNumTex->change_text(s);
    }
    moneyNumTex->render();
    TechNumTex->render();
    moneyTex->render();
    techTex->render();
    if(gameMain->timeAccel == 0)
    {
        pauseTex->render();
    }
    else
    {
        playTex->texCoords = timeHits[0];
        for(int i = 0;i < gameMain->timeAccel;i++)
        {
			playTex->texCoords = timeHits[i];
            playTex->render();
        }
    }
    for(int i = 0;i < buttons[subMain->station->stage_type].size();i++)
    {
        buttons[subMain->station->stage_type][i]->render();
    }
    rTex->render();
    mTex->render();
}

void OverlayInfoBanner::change(int v)
{

}



























