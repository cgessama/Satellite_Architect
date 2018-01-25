#include "SourceGame/subResearch.h"
#include "SourceStates/game_state.h"
#include "SourceResearch/Option.h"
#include "SourceResearch/option_enums.h"
#include "SourceCore/colide.h"

SubResearch::SubResearch()
{
    bkg = new Image(0,0,camW,camH,0,"research/bkg.png",0,0);
    queueHighlight = new Image(-.1,-.1,400,200,0,"research/queueHighlight.png",0,0);
    queueNumber = new Text(250,45,0,0,"1",0,0,200,false);
    nameTex = new Text(1000,50,1530,5,"Research",0,0,0,false);
    techTex = new Text(1000,50,645,5,"Intel: Bugged",0,0,0,false);
    intelCurrent = false;
    backTex = new Text(1000,50,10,5,"<---",0,0,0,false);
    //x off for options is 400
    techOptions.push_back(new TechOption("Basic Inteligence",500,550,100,OptionType::BASIC_INTELIGENCE));
    techOptions.push_back(new TechOption("Station Management",900,550,200,OptionType::STATION_MANAGEMENT));//340 is option width
    techOptions[1]->preReq.push_back(techOptions[0]);
    techOptions.push_back(new TechOption("Solar Cells"       ,1300,350,1000,OptionType::SOLAR_CELLS));//340 is option width
    techOptions[2]->preReq.push_back(techOptions[1]);
    techOptions.push_back(new TechOption("Advanced Mechanics",1300,750,1000,OptionType::ADVANCED_MECHANICS));//340 is option width
    techOptions[3]->preReq.push_back(techOptions[1]);
    techOptions.push_back(new TechOption("Matter Synthesis"  ,1700,550,5000,OptionType::MATTER_SYNTHESIS));//340 is option width
    techOptions[4]->preReq.push_back(techOptions[2]);
    techOptions[4]->preReq.push_back(techOptions[3]);
    //make sure these line up with enums
}

void SubResearch::handle_events()
{
    if(event.type == SDL_KEYUP)
    {
        switch(event.key.keysym.sym)
        {
        case 8://bckspc
            gameMain->subActive = subMain;
            break;
        }
    }
    else if(event.type == SDL_MOUSEBUTTONDOWN)
    {
        bool done = false;
        for(int i = 0;i < techOptions.size();i++)
        {
            if(lp_point(mousePos,*techOptions[i]->bkg->texCoords))
            {
                if(techOptions[i]->in_queue)
                {
                    remove_from_queue(techOptions[i]);
                }
                else
                {
                    add_to_queue(techOptions[i]);
                }
                done = true;
                break;
            }
        }
        if(!done && lp_point(mousePos,*backTex->texCoords))
        {
            gameMain->subActive = subMain;
            intelCurrent = false;
        }
    }
}

void SubResearch::add_to_queue(TechOption * t)
{
    if(t->completion < t->cost)
    {
        bool notAlready = true;
        for(int i = 0;i < queue.size();i++)
        {
            if(queue[i] == t)
            {
                notAlready = false;
                break;
            }
        }
        if(notAlready)//is not in queue yet
        {
            if(!t->eval_prereq())
            {
                for(int i = 0;i < t->preReq.size();i++)
                {
                    add_to_queue(t->preReq[i]);
                }
            }
            queue.push_back(t);
            t->in_queue = true;
        }
    }

}

void SubResearch::remove_from_queue(TechOption * t)
{
    t->in_queue = false;
    for(int i = 0;i < queue.size();i++)
    {
        if(queue[i] == t)
        {
            queue.erase(queue.begin() + i);
            break;
        }
    }
    fix_queue();
}

void SubResearch::fix_queue()
{
    for(int i = 0;i < queue.size();i++)
    {
        for(int j = 0;j < queue[i]->preReq.size();j++)
        {
            if(!queue[i]->preReq[j]->in_queue && queue[i]->preReq[j]->completion < queue[i]->preReq[j]->cost)
            {
                remove_from_queue(queue[i]);
                return;
            }
        }
    }
}

void SubResearch::logic()
{
	std::string s;
	if (queue.size() > 0)
	{
		s = "Tech: " + num_to_string((int)queue[0]->completion) + "/" + num_to_string(queue[0]->cost);
	}
	else 
	{
		s = "No Research";
	}
    if(s != techTex->text)
    {
        techTex->change_text(s);
    }
    for(int i = 0;i < techOptions.size();i++)
    {
        techOptions[i]->logic();
    }
}

void SubResearch::render()
{
    glLoadIdentity();
    bkg->render();
    nameTex->render();
    backTex->render();
    techTex->render();
    for(int i = 0;i < queue.size();i++)
    {
        queueHighlight->texCoords->orig = queue[i]->bkg->texCoords->orig;
        queueHighlight->render();
    }
    for(int i = 0;i < techOptions.size();i++)
    {
        techOptions[i]->render();
    }
    for(int i = 0;i < queue.size();i++)
    {
        queueNumber->texCoords->orig = queue[i]->bkg->texCoords->orig + Point(60,130);
        std::string s = num_to_string(i + 1);
        if(queueNumber->text != s)
        {
            queueNumber->change_text(s);
        }
        queueNumber->render();
    }
}













