#include "SourceAstronauts/team.h"
#include "SourceAstronauts/astronaut.h"
#include "SourceStations/stations.h"
#include "SourceGame/subMain.h"

Team::Team(float a)
{
    loc = Point(0,0);
    if(a == 1)
    {
        members.push_back(new Astronaut);
        members[members.size() - 1]->memberOf = this;
        members[members.size() - 1]->tex->texCoords->orig = loc;
    }
    else
    {
        Point aLoc(40 / std::cos(((180 - (360 / a)) / 2) / 57.2957795),0);//40 is the radius of space around each astronaut
        //57.2957795 converts to radians
        /*//less compressed version
        float degrees = (180 - (360 / a)) / 2;
        Point aLoc(40 / std::cos(degrees / 57.2957795),0);//20 is the radius of space around each astronaut
        //57.2957795 converts to radians
        */
        for(int i = 0;i < a;i++)
        {
            members.push_back(new Astronaut);
            members[members.size() - 1]->memberOf = this;
            members[members.size() - 1]->tex->texCoords->orig = loc + aLoc;
            aLoc.rotate(360 / a);
        }
    }
    rotation = 0;
    rotate(subMain->station->gameSpace->rot);//allign with window
}

Team::~Team()
{
	for (int i = 0; i < members.size(); i++) {
		delete members[i];
	}
    members.clear();
    for(int i = 0;i < subMain->station->teams.size();i++)
    {
        if(subMain->station->teams[i] == this)
        {
            subMain->station->teams.erase(subMain->station->teams.begin() + i);
            break;
        }
    }
}

void Team::logic()
{
    for(int i = 0;i < members.size();i++)
    {
        members[i]->logic();
    }
}

void Team::render()
{
    for(int i = 0;i < members.size();i++)
    {
        members[i]->render();
    }
}

void Team::rotate(float r)
{
    for(int i = 0;i < members.size();i++)
    {
        members[i]->tex->texCoords->rotate(r);
        members[i]->tex->texCoords->orig = members[i]->tex->texCoords->orig - loc;
        members[i]->tex->texCoords->orig.rotate(r);
        members[i]->tex->texCoords->orig = members[i]->tex->texCoords->orig + loc;
    }
    rotation += r;
}

void Team::move(float x,float y)
{
    loc.x += x;
    loc.y += y;
    for(int i = 0;i < members.size();i++)
    {
        members[i]->tex->texCoords->orig.x += x;
        members[i]->tex->texCoords->orig.y += y;
    }
}

void Team::move_to(float x,float y)
{
    Point dif = Point(x,y) - loc;
    loc = Point(x,y);
    for(int i = 0;i < members.size();i++)
    {
        members[i]->tex->texCoords->orig = members[i]->tex->texCoords->orig + dif;
    }
}

void Team::extend(Team * t)
{
    for(int i = 0;i < t->members.size();i++)
    {
        members.push_back(t->members[i]);
        members[members.size() - 1]->memberOf = this;
        members[members.size() - 1]->inside = inside;
        members[members.size() - 1]->tex->texCoords->rotate(rotation - t->rotation);
    }
    for(int i = 0;i < subMain->station->teams.size();i++)
    {
        if(subMain->station->teams[i] == t)
        {
            subMain->station->teams.erase(subMain->station->teams.begin() + i);
        }
    }
    t->members.clear();
    delete t;
    Point aLoc(40 / std::cos(((180 - (360 / (float)members.size())) / 2) / 57.2957795),0);//40 is the radius of space around each astronaut
    //57.2957795 converts to radians
    /*//less compressed version
    float degrees = (180 - (360 / a)) / 2;
    Point aLoc(40 / std::cos(degrees / 57.2957795),0);//20 is the radius of space around each astronaut
    //57.2957795 converts to radians
    */
    for(int i = 0;i < members.size();i++)
    {
        members[i]->tex->texCoords->orig = loc + aLoc;
        aLoc.rotate(360 / (float)members.size());
    }
}


