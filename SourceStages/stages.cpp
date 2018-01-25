#include "SourceStages/stages.h"

STGOrbit * sTGOrbit;
STGGround * sTGGround;

Stage::Stage()
{
    bkg = NULL;
    default_temp = 0;
    default_oxy = 0;
}

STGOrbit::STGOrbit()
{
    bkg = new Image(-2300 / 2,-2300 / 2,2300,2300,0,"BKGs/orbit.png",0,0);
    default_temp = 0;
    default_oxy = 0;
	resourceCosts.push_back(10);
	resourceCosts.push_back(20);//lines up with ResourceType
}

STGGround::STGGround()
{
    bkg = new Image(-400 / 2,-400 / 2,400,400,0,"BKGs/ground.png",0,0);
    ocean = new Image(-400 / 2,-400 / 2,400,400,0,"BKGs/ocean.png",0,0);
    beach = new Image(-8000 / 2,-8000 / 2,8000,8000,0,"BKGs/sand.png",0,0);
    default_temp = 20;
    default_oxy = 100;
	resourceCosts.push_back(5);
	resourceCosts.push_back(10);//lines up with ResourceType
}

void STGGround::render_bkg()
{
    float tex_width = bkg->texCoords->vert[1].x - bkg->texCoords->vert[0].x;
    Point tile_transx(tex_width,0);
    Point tile_transy(0,tex_width);
    float max_dist = subMain->station->gameSpace->vert[0].get_dist();
    float tex_dist = Point(tex_width / 2,tex_width / 2).get_dist();
    float iterations = int(max_dist / tex_width) + 2;
    glLoadIdentity();
    glTranslatef(camW / 2,camH / 2,0);
    //glScalef(1 / (gameSpaceScale * 2),1 / (gameSpaceScale * 2),1); test feature for scrolling bkg
    glScalef(1 / subMain->station->gameSpaceScale,1 / subMain->station->gameSpaceScale,1);
    glRotatef(-subMain->station->gameSpace->rot,0,0,1);
    glTranslatef(-subMain->station->gameSpace->orig.x,-subMain->station->gameSpace->orig.y,0);
    Point tileLoc(0,0);
    for(int i = 0;i < iterations * 2;i++)
    {
        for(int j = 0;j < iterations * 2;j++)
        {
            tileLoc = (subMain->station->gameSpace->orig - Point(fmod(subMain->station->gameSpace->orig.x,tex_width),fmod(subMain->station->gameSpace->orig.y,tex_width)))//move with an offset so it appears that you do not
                                     + (tile_transx * (i - iterations)) + (tile_transy * (j - iterations));//tile
            if(tileLoc.get_dist() < 3600)
            {
                bkg->texCoords->orig = tileLoc;
                bkg->render();
            }
            else
            {
                ocean->texCoords->orig = tileLoc;
                ocean->render();
            }
        }
    }
    beach->render();
}






















