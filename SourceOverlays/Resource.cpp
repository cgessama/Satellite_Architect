#include "SourceOverlays/Overlays.h"
#include "SourceCore/colide.h"

OverlayResource::OverlayResource()
{
    loc = Point(800,500);
    bkg = new Image(-.1,-.1,400,350,0,"Overlays/resource.png",loc.x,loc.y);
    nameTex = new Text(1000,40,loc.x + 40,loc.y + 30,"Resources",0,0,0,false);
    type = OverlayType::RESOURCE;
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
    hits[1]->vert.push_back(Point(190,-30));//Diagonal
    hits[1]->vert.push_back(Point(230,20));//line
    hits[1]->vert.push_back(Point(-25,20));
    hits[1]->vert.push_back(Point(-25,-5));
    hits[1]->fix();

    names.push_back(new Text(330,35,loc.x + 40,loc.y + 115 + (35 * 0),"Food:"     ,0,0,0,false));
    nums.push_back(new Text(200,35,loc.x + 275,loc.y + 115 + (35 * 0) + (35 / 2),"-1",0,0,0,true));
    names.push_back(new Text(330,35,loc.x + 40,loc.y + 115 + (35 * 1),"Duct tape:",0,0,0,false));
    nums.push_back(new Text(200,35,loc.x + 275,loc.y + 115 + (35 * 1) + (35 / 2),"-1",0,0,0,true));
}

void OverlayResource::logic()
{
    if(pickedUp)
    {
        Point temp = mousePos - bkg->texCoords->orig + offset;
        move(temp.x,temp.y);
    }
}

bool OverlayResource::handle_click()
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

void OverlayResource::render()
{
    update_texes();
    bkg->render();
    nameTex->render();
    for(int i = 0;i < names.size();i++)
    {
        names[i]->render();
    }
    if(subMain->station != NULL)
    {
        for(int i = 0;i < nums.size();i++)
        {
            nums[i]->render();
        }
    }
}

void OverlayResource::move(float x,float y)
{
    Point oldLoc = loc;
    loc = Point(loc.x + x,loc.y + y);
    nameTex->texCoords->orig = Point(loc.x + 40,loc.y + 30);
    bkg->texCoords->orig = loc;

    for(int i = 0;i < names.size();i++)
    {
        names[i]->texCoords->orig = loc + (names[i]->texCoords->orig - oldLoc);
        nums[i]->texCoords->orig = loc + (nums[i]->texCoords->orig - oldLoc);
    }

    hits[0]->orig = Point(loc.x + 20,loc.y + 60);
    hits[1]->orig = Point(loc.x + 50,loc.y + 50);
}

void OverlayResource::update_texes()
{
    if(subMain->station != NULL)
    {
        std::string s;
        for(int i = 0;i < names.size();i++)
        {
            s = num_to_string(int(subMain->station->resources[i]));
            if(nums[i]->text != s)
            {
                nums[i]->change_text(s);
            }
        }
    }
}











