#ifndef OPTION_H
#define OPTION_H

#include "SourceCore/engine.h"

class TechOption
{
public:
    Point loc;
    Texture * bkg;
    Text * nameTex;
    Text * progressTex;
    float cost; // int intel points
    float completion;// number of science points (displayed as percent)
    bool in_queue;
    int type;//enum type

    std::vector<TechOption *> preReq;

    TechOption(std::string,float,float,float,int);
    void logic();
    void render();
    void render_connection();
    bool eval_prereq();
    void eval_completion();
};

#endif // OPTION_H
