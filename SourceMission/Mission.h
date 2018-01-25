#ifndef MISSION_H
#define MISSION_H
#include "SourceCore/engine.h"
#include "SourceMission/Case.h"

class Mission
{
public:
    Point loc;
    Case * condition;
    Text * wordTex;
    Text * rewardTex;
    float reward;
    bool completion;
    Mission(float,float,std::string,int,float);
    void eval();
    void render();
};

#endif // MISSION_H
