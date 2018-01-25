
#include "SourceStates/game_state.h"
#include "SourceModule/modules.h"
#include "SourceResearch/Option.h"
#include "SourceResearch/option_enums.h"
#include "SourceAstronauts/team.h"


State * active;//here because it cant be used in head and this will be the most active location
Texture * mouse;
GameState * gameMain;
SubMain * subMain;
SubResearch * subResearch;
SubMission * subMission;

GameState::GameState()
{
    keyStates = SDL_GetKeyboardState(NULL);
    subResearch = new SubResearch;
    subMission = new SubMission;
    subMain = new SubMain;
    subActive = subMain;
    //money and time accel set in un_load
    un_load_save();
}

void GameState::handle_events()
{
    subActive->handle_events();
}

void GameState::logic()
{
    subActive->logic();
}

void GameState::render()
{
    subActive->render();
    glLoadIdentity();
}







































