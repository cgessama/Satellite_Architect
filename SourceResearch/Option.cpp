#include "SourceGame/subResearch.h"
#include "SourceResearch/Option.h"
#include "SourceStates/game_state.h"

TechOption::TechOption(std::string name,float x,float y,float c,int t)
{
    loc = Point(x - 370,y - 120);
    bkg = new Image(-.1,-.1,400,200,0,"research/option.png",loc.x,loc.y);
    nameTex = new Text(250,35,loc.x + 60,loc.y  + 20,name.c_str(),0,0,0,false);
    completion = 0;
    cost = c;
    progressTex = new Text(250,27,loc.x + 60,loc.y + 60,"0/" + num_to_string(cost),255,0,0,false);
    in_queue = false;
    type = t;
}

void TechOption::logic()
{
    if(completion >= cost)
    {
        if(progressTex->text != "COMPLETED")
        {
            progressTex->change_text("COMPLETED");
            progressTex->color.r = 0;
            progressTex->color.g = 255;
        }
    }
    else
    {
        std::string s;
        s = num_to_string(int(completion)) + "/" + num_to_string((int)cost);
        if(progressTex->text != s)
        {
            progressTex->change_text(s);
        }
    }
}

void TechOption::render()
{
    render_connection();
    bkg->render();
    nameTex->render();
    progressTex->render();
}

void TechOption::render_connection()
{
    for(int i = 0;i < preReq.size();i++)
    {
        glBegin(GL_QUADS);//default is white
        glVertex2f(loc.x + 33,loc.y + 118);
        glVertex2f(loc.x + 33,loc.y + 124);
        glVertex2f(preReq[i]->loc.x + 366,preReq[i]->loc.y + 124);
        glVertex2f(preReq[i]->loc.x + 366,preReq[i]->loc.y + 118);
        glEnd();
    }

}

bool TechOption::eval_prereq()
{
    for(int i = 0;i < preReq.size();i++)
    {
        if(preReq[i]->completion < preReq[i]->cost)
        {
            return false;
        }
    }
    return true;
}

void TechOption::eval_completion()
{
    if(completion >= cost)
    {
        subMain->create_popups("Tech " + nameTex->text + " has been completed.");
        subResearch->queue.erase(subResearch->queue.begin());
    }
}































