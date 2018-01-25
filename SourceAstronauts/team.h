#ifndef TEAM_H
#define TEAM_H

#include "SourceCore/engine.h"

class Astronaut;
class Module;

class Team
{
public:
    std::vector<Astronaut *> members;
    Team(float);
    Module * inside;
    Point loc;
    float rotation;//so that when teams are combined the astronauts can be aligned
    void logic();
    void render();
    void rotate(float);
    void move(float,float);
    void move_to(float,float);
    void extend(Team *);
	~Team();
};

#endif // TEAM_H

