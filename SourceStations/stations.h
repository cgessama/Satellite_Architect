#ifndef STATIONS_H
#define STATIONS_H

#include "SourceCore/engine.h"

class Module;
class Stage;
class Team;

class Station
{
public:
    Station(int);
    ~Station();
    std::vector<Module *> modules;
    std::vector<Team *> teams;
    int stage_type;//0 = orbital,1 = ground,
    Stage * stage;
    float gameSpaceScale;//-------------------------------------------------------------------gameCore
    HitBox * gameSpace;
    float energyProduced;
    float energyConsumed;
    float energyRequested;
    float energyStored;
    float totalEnergyStorage;
    float energyFactor;//the percentage requested that will be used
    float energyStorageFactor;
    void logic();
};



#endif // STATIONS_H
