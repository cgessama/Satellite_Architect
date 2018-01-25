#ifndef STAGE_H
#define STAGE_H
#include "SourceCore/engine.h"
#include "SourceStations/stations.h"
#include "SourceAstronauts/astronaut.h"
#include "SourceGame/subMain.h"
#include "SourceCore/globals.h"

class Stage
{
public:
    Texture * bkg;
    float default_temp;
    float default_oxy;
	std::vector<float> resourceCosts;
    Stage();
    virtual void render_bkg()
    {
        glLoadIdentity();
        glTranslatef(camW / 2,camH / 2,0);
        glRotatef(-subMain->station->gameSpace->rot,0,0,1);
        bkg->render();
    }
};

class STGOrbit: public Stage
{
public:
    STGOrbit();
};

class STGGround: public Stage
{
public:
    STGGround();
    Texture * ocean;
    Texture * beach;
    void render_bkg();
};

extern STGOrbit * sTGOrbit;
extern STGGround * sTGGround;

#endif // STAGE_H
