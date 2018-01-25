#include "SourceGame/subMission.h"
#include "SourceStates/game_state.h"
#include "SourceMission/Mission.h"

SubMission::SubMission()
{
    missionBkg = new Image(-.1,-.1,1720,300,0,"Mission/missionBkg.png",0,0);
    missions.push_back(new Mission(100,200,"Get three astronauts in space and happy",1,1000));
    missionCompleteTex = new Text(1000,50,0,0,"Mission Complete",0,255,0,false);
}

void SubMission::handle_events()
{
    if(event.type == SDL_KEYUP)
    {
        switch(event.key.keysym.sym)
        {
        case 8://bckspc
            gameMain->subActive = subMain;
            break;
        default:
            break;
        }
    }
}

void SubMission::logic()
{

}

void SubMission::render()
{
    glLoadIdentity();
    render_rect(0,0,1920,1080,0,0,0,1);
    for(int i = 0;i < missions.size();i++)
    {
        missions[i]->render();
    }
}
