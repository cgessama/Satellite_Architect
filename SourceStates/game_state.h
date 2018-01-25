#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "SourceStates/states.h"
#include "SourceGame/subMain.h"
#include "SourceGame/subResearch.h"
#include "SourceGame/subMission.h"
#include "SourceStages/stages.h"


class GameState: public State
{
public:
    SubState * subActive;

    const Uint8 * keyStates;//---------------------------------------------------------------Global

    float money;

    float timeAccel;

    GameState();

    void handle_events();
    void logic();
    void render();//-----------------------------------------------------------------------------render()

    void save(std::string);//-------------------------------------------------------------------------------saves
	void load_save(std::string);
    int find_line(std::vector<std::string>,std::string);//returns the line that the category passed as a parameter starts
    float load_aspect(std::string);
    void load_game_data(std::vector<std::string>);
    void load_tech_stats(std::vector<std::string>);
    void load_tech_queue(std::vector<std::string>);
    void load_station(std::vector<std::string>);
    void load_module(std::vector<std::string>);
    void load_team(std::vector<std::string>);
    void un_load_save();
}
extern * gameMain;//one offs have to be pointers so that the implemention can be in the constructor

#endif // GAME_STATE_H
