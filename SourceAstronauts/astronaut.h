#ifndef ASTRONAUT_H
#define ASTRONAUT_H

#include "SourceCore/engine.h"

class Module;
class Team;

class Astronaut
{
public:
    Texture * tex;
    std::vector<Picture *> overlays;
    Module * inside;
    Team * memberOf;
    Astronaut();
    void logic();
    void render();
    ~Astronaut();
};

#endif // ASTRONAUT_H
