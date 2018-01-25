#include "SourceOverlays/Overlays.h"
#include "SourceModule/modules.h"
#include "SourceCore/colide.h"

OverlayDetailPanel::OverlayDetailPanel(int d,float lv,float hv,SDL_Color lc,SDL_Color hc,std::string n,int t)
{
    detail = d;
    lowest_val = lv;
    highest_val = hv;
    low_c = lc;
    high_c = hc;
    loc = Point(1000,300);
    nameTex = new Text(500,50,loc.x - 345 + 45,loc.y - 237.5 + 25,n,0,0,0,false);
    bkg = new Image(-345,-237.5,690,475,0,"Overlays/detail.png",loc.x,loc.y);
    type = t;
    hits.push_back(new HitBox(-325,-152.5,650,370,loc.x,loc.y,0));
    hits.push_back(new HitBox());
    hits[1]->orig = Point(loc.x - 295,loc.y - 187.5);
    hits[1]->vert.push_back(Point(-6,-30));
    hits[1]->vert.push_back(Point(178,-30));
    hits[1]->vert.push_back(Point(242,35));
    hits[1]->vert.push_back(Point(-30,35));
    hits[1]->vert.push_back(Point(-30,-6));
    hitsInclusive.push_back(true);
    hitsInclusive.push_back(true);
    pickedUp = false;
    offset = Point(0,0);
    change(0);
}

void OverlayDetailPanel::render()
{
    bkg->render();
    nameTex->render();
    glViewport(veiwSpaceLoc.x,veiwSpaceLoc.y,veiwSpaceDim.x,veiwSpaceDim.y);
    render_rect(0,0,camW,camH,0,0,0,1);

    glLoadIdentity();
    glTranslatef(camW / 2,camH / 2,0);
    glScalef(1 / subMain->station->gameSpaceScale,1 / subMain->station->gameSpaceScale,1);
    glRotatef(-subMain->station->gameSpace->rot,0,0,1);
    glTranslatef(-subMain->station->gameSpace->orig.x,-subMain->station->gameSpace->orig.y,0);
    float outlineWidth = subMain->station->gameSpaceScale / 10;
    if(outlineWidth < .03)
    {
        outlineWidth = .03;
    }
    for(int i = 0;i < subMain->station->modules.size();i++)
    {
        if(subMain->station->modules[i]->volume)
        {
            float ratio = (subMain->station->modules[i]->details[detail]->value - lowest_val) / (highest_val - lowest_val);
            if(ratio > 1)
            {
                ratio = 1;
            }
            else if(ratio < 0)
            {
                ratio = 0;
            }
            subMain->station->modules[i]->hit->render(ratio * ((float)high_c.r / 255) + (1.0 - ratio) * ((float)low_c.r / 255),
                                                         ratio * ((float)high_c.g / 255) + (1.0 - ratio) * ((float)low_c.g / 255),
                                                         ratio * ((float)high_c.b / 255) + (1.0 - ratio) * ((float)low_c.b / 255),1);
        }
        subMain->station->modules[i]->draw_outline(1,1,1,1,outlineWidth);
    }
    //test feature
    /*Point temp(0,0);
    for(int i = 0;i < 1920 / 20;i++)
    {
        for(int j = 0;j < 1080 / 20;j++)
        {
            temp.x = i * 20;
            temp.y = j * 20;
            temp.render_p(NULL);
        }
    }*/
    glViewport(windowOffX,windowOffY,viewW,viewH);
    glLoadIdentity();
}

void OverlayDetailPanel::change(int v)
{
    if(v == 0)
    {
        veiwSpaceDim = Point(viewW / 3,viewH / 3);
        veiwSpaceLoc = Point((loc.x - 345 + 25) * (viewW / camW) + windowOffX,(1080 - (loc.y - 237.5 + 90)) * (viewH / camH)  - veiwSpaceDim.y + windowOffY);
    }
}

void OverlayDetailPanel::move3(float x,float y)
{
    loc = Point(loc.x + x,loc.y + y);
    nameTex->texCoords->orig = Point(loc.x - 345 + 45,loc.y - 237.5 + 25);
    bkg->texCoords->orig = loc;
    hits[0]->orig = loc;
    hits[1]->orig = Point(loc.x - 295,loc.y - 187.5);
    change(0);
}

bool OverlayDetailPanel::handle_click()
{
    if(lp_point(mousePos,*hits[0]) == hitsInclusive[0] || lp_point(mousePos,*hits[1]) == hitsInclusive[1])
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

void OverlayDetailPanel::logic()
{
    if(pickedUp)
    {
        Point temp = mousePos - bkg->texCoords->orig + offset;
        move3(temp.x,temp.y);
    }
}
