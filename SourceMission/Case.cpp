#include "SourceMission/Case.h"
#include "SourceGame/subMain.h"
#include "SourceAstronauts/astronaut.h"
#include "SourceStages/stages.h"
#include "SourceAstronauts/team.h"

Case3Astros case3Astros;

bool Case3Astros::eval()
{
    int x = 0;
    for(int i = 0;i < subMain->stations[0]->teams.size();i++)
    {
        for(int j = 0;j < subMain->stations[0]->teams[i]->members.size();j++)
        {
            if(subMain->stations[0]->teams[i]->members[j]->overlays.size() == 0)//astronaut is happy
            {
                x++;
            }
            if(x == 3)
            {
                return true;
            }
        }

    }
    return false;
}
