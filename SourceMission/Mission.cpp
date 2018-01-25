#include "SourceMission/Mission.h"
#include "SourceStates/game_state.h"

Mission::Mission(float x,float y,std::string s,int c,float r)
{
    loc.x = x;
    loc.y = y;
    wordTex = new Text(1600,100,x + 50,y + 50,s,0,0,0,false);
    if(c == 1)
    {
        condition = &case3Astros;
    }
    completion = false;
    reward = r;
    rewardTex = new Text(1000,50,loc.x + 200,loc.y + 200,"Reward: $" + num_to_string(r),0,255,0,false);
}

void Mission::eval()
{
    if(!completion && condition->eval())
    {
        gameMain->money += reward;
        subMain->create_popups("Mission Completed +" + num_to_string(reward) + "$");
        completion = true;
    }
}

void Mission::render()
{
    subMission->missionBkg->texCoords->orig = loc;
    subMission->missionBkg->render();
    if(completion)
    {
        subMission->missionCompleteTex->texCoords->orig = loc + Point(1000,200);
        subMission->missionCompleteTex->render();
    }
    rewardTex->render();
    wordTex->render();
}
