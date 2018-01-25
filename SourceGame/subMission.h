#ifndef SUBMISSION_H
#define SUBMISSION_H

#include <vector>
#include "SourceGame/subState.h"
class Texture;
class Text;

class Mission;

class SubMission: public SubState
{
public:
    std::vector<Mission *> missions;
    Texture * missionBkg;
    Text * missionCompleteTex;
    SubMission();
    void handle_events();
    void logic();
    void render();
}extern * subMission;

#endif // SUBMISSION_H

