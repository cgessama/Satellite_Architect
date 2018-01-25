#include "SourceStates/game_state.h"
#include "SourceResearch/Option.h"
#include "SourceAstronauts/team.h"
#include "SourceModule/modules.h"
#include "SourceResearch/option_enums.h"
#include "SourceModule/detail_enums.h"
#include "SourceStations/stations.h"
#include "SourceOverlays/enums.h"
#include "SourceCore/colide.h"
#include "SourceMission/Mission.h"

void GameState::save(std::string saveName)
{
    std::ofstream file;
    std::string s = "saves/" + saveName + ".txt";
    file.open(s.c_str());//opens and empties or creates the file
    file << "Game Data: \n";
    file << '\t' << "Money: " << money << "\n";
    file << '\t' << "Time Speed: " << timeAccel << "\n";
    file << "Tech Stats: \n";
    for(int i = 0;i < subResearch->techOptions.size();i++)
    {
        file << '\t' << subResearch->techOptions[i]->nameTex->text << ": " << subResearch->techOptions[i]->completion << '\n';
    }
    file << "Tech Queue: \n";
    for(int i = 0;i < subResearch->queue.size();i++)
    {
        file << '\t' << subResearch->techOptions[i]->nameTex->text << ": " << subResearch->queue[i]->type << '\n';
    }
    file << "Tech Queue End: \n";
    file << "Stations: \n";
    for(int i = 0;i < subMain->stations.size();i++)
    {
        file << "\tStation: " << i << "\n";
        file << "\tStage: " << subMain->stations[i]->stage_type << "\n";
        file << "\t\tModules: \n";
        for(int j = 0;j < subMain->stations[i]->modules.size();j++)
        {
            subMain->stations[i]->modules[j]->save(&file,j);
        }
        file << "\t\tModules End: \n";
        file << "\t\tTeams: \n";
        for(int j = 0;j < subMain->stations[i]->teams.size();j++)
        {
            file << "\t\t\tTeam: " << j << '\n';
            file << "\t\t\t\tLocation X: " << subMain->stations[i]->teams[j]->loc.x << "\n";
            file << "\t\t\t\tLocation X: " << subMain->stations[i]->teams[j]->loc.y << "\n";
            file << "\t\t\t\tRotation: " << subMain->stations[i]->teams[j]->rotation << "\n";
            file << "\t\t\t\tMembers: " << subMain->stations[i]->teams[j]->members.size() << "\n";
        }
        file << "\t\tTeams End: \n";
        file << "\t\tGameSpace: \n";
        file << "\t\t\tLocation X: " << subMain->stations[i]->gameSpace->orig.y << '\n';
        file << "\t\t\tLocation Y: " << subMain->stations[i]->gameSpace->orig.y << '\n';
        file << "\t\t\tScale: " << subMain->stations[i]->gameSpaceScale << '\n';
        file << "\t\t\tRotation: " << subMain->stations[i]->gameSpace->rot << '\n';
    }
    file << "Stations End: \n";
    file << "Missions: \n";
    for(int i = 0;i < subMission->missions.size();i++)
    {
        file << "\tMission: " << i << " \n\t\t";
        if(subMission->missions[i]->completion)
        {
            file << "Completed: 1\n";
        }
        else
        {
            file << "Incomplete: 0\n";
        }
    }
    file << "end";
    file.close();
}

void GameState::load_save(std::string saveName)
{
    std::ifstream file;
    std::string s = "saves/" + saveName + ".txt";
    file.open(s.c_str());//opens or creates the file
    std::string line;
    std::vector<std::string> lines;
    while(std::getline(file,line))
    {
        lines.push_back(line);
    }
    load_game_data(lines);
    load_tech_stats(lines);
    load_tech_queue(lines);
    std::vector<int> stationBoundries;//indexes of the lines on which stations start and end
    for(int i = 0;true;i++)
    {
        stationBoundries.push_back(find_line(lines,"\tStation: " + num_to_string(i)));
        if(stationBoundries.back() == -1)
        {
            stationBoundries.back() = find_line(lines,"Stations End: ");
            break;
        }
    }
    std::vector<std::string> temp;
    for(int i = 0;i < stationBoundries.size() - 1;i++)
    {
        for(int j = 0;j < stationBoundries[i + 1] - stationBoundries[i];j++)
        {
            temp.push_back(lines[stationBoundries[i] + j]);
        }
        load_station(temp);
        temp.clear();
    }
    subMission->missions[0]->completion = (bool)load_aspect(lines[find_line(lines,"\tMission: 0") + 1]);
}

int GameState::find_line(std::vector<std::string> lines,std::string s)
{
    for(int i = 0;i < lines.size();i++)
    {
        if(lines[i] == s)
        {
            return i;
        }
    }
    return -1;
}

float GameState::load_aspect(std::string s)
{
    int num_start = s.find_first_of(":") + 2;//1 for space and 1 for block math
    std::string valueString;
    for(int i = num_start;i < s.size();i++)
    {
        valueString += s[i];
    }
    return string_to_float(valueString);
}

void GameState::load_game_data(std::vector<std::string> lines)
{
    int start = find_line(lines,"Game Data: ");
    money = load_aspect(lines[start + 1]);
    timeAccel = (int)load_aspect(lines[start + 2]);
}

void GameState::load_tech_stats(std::vector<std::string> lines)
{
    int start = find_line(lines,"Tech Stats: ");
    for(int i = 0;i < subResearch->techOptions.size();i++)
    {
        subResearch->techOptions[i]->completion = load_aspect(lines[start + i + 1]);
    }
}

void GameState::load_tech_queue(std::vector<std::string> lines)
{
    int start = find_line(lines,"Tech Queue: ");
    int end = find_line(lines,"Tech Queue End: ");
    for(int i = start + 1;i < end;i++)
    {
        subResearch->add_to_queue(subResearch->techOptions[load_aspect(lines[start + 1])]);
    }
}

void GameState::load_station(std::vector<std::string> trimmedLines)
{
    /*for(int i = 0;i < trimmedLines.size();i++)
    {
        std::cot << trimmedLines[i] << '\n';
    }
    std::cot << "\n\n\n\n";*/
    //print stuff
    subMain->stations.push_back(new Station(load_aspect(trimmedLines[1])));
    subMain->station = subMain->stations.back();
    //modules
    std::vector<int> aspectBoundries;//indexes of the lines on which stations start and end
    for(int i = 0;true;i++)
    {
        aspectBoundries.push_back(find_line(trimmedLines,"\t\t\tModule: " + num_to_string(i)));
        if(aspectBoundries.back() == -1)
        {
            aspectBoundries.back() = find_line(trimmedLines,"\t\tModules End: ");
            break;
        }
    }
    std::vector<std::string> temp;
    for(int i = 0;i < aspectBoundries.size() - 1;i++)
    {
        for(int j = 0;j < aspectBoundries[i + 1] - aspectBoundries[i];j++)
        {
            temp.push_back(trimmedLines[aspectBoundries[i] + j]);
        }
        load_module(temp);
        temp.clear();
    }
    //teams
    temp.clear();
    aspectBoundries.clear();
    for(int i = 0;true;i++)
    {
        aspectBoundries.push_back(find_line(trimmedLines,"\t\t\tTeam: " + num_to_string(i)));
        if(aspectBoundries.back() == -1)
        {
            aspectBoundries.back() = find_line(trimmedLines,"\t\tTeams End: ");
            break;
        }
    }
    for(int i = 0;i < aspectBoundries.size() - 1;i++)
    {
        for(int j = 0;j < aspectBoundries[i + 1] - aspectBoundries[i];j++)
        {
            temp.push_back(trimmedLines[aspectBoundries[i] + j]);
        }
        load_team(temp);
        temp.clear();
    }
    //load gameSpace
    int start = find_line(trimmedLines,"\t\tGameSpace: ");
    subMain->station->gameSpace->orig = Point(load_aspect(trimmedLines[start + 1]),load_aspect(trimmedLines[start + 2]));
    subMain->station->gameSpace->scale(load_aspect(trimmedLines[start + 3]));
    subMain->station->gameSpaceScale = load_aspect(trimmedLines[start + 3]);
    subMain->station->gameSpace->rotate(load_aspect(trimmedLines[start + 4]));
}

void GameState::load_module(std::vector<std::string> trimmedLines)//assumes station is already assigned
{
    /*for(int i = 0;i < trimmedLines.size();i++)
    {
        std::cot << trimmedLines[i] << '\n';
    }
    std::cot << "\n\n\n\n";*/
    //print stuff
    new_module(load_aspect(trimmedLines[1]));
    subMain->inHand[0]->details[DetailType::OXYGEN]->value = load_aspect(trimmedLines[2]);
    subMain->inHand[0]->details[DetailType::TEMPERATURE]->value = load_aspect(trimmedLines[3]);
    subMain->inHand[0]->details[DetailType::INTEGRITY]->value = load_aspect(trimmedLines[4]);
    subMain->inHand[0]->energyStored = load_aspect(trimmedLines[5]);
    subMain->inHand[0]->translate_m(load_aspect(trimmedLines[6]),load_aspect(trimmedLines[7]),true);
    subMain->inHand[0]->rotate(load_aspect(trimmedLines[8]));
    subMain->handle_placement();
}

void GameState::load_team(std::vector<std::string> trimmedLines)//assumes station is already assigned
{
    /*for(int i = 0;i < trimmedLines.size();i++)
    {
        std::cot << trimmedLines[i] << '\n';
    }
    std::cot << "\n\n\n\n";*/
    //print stuff
    subMain->inHandA = new Team(load_aspect(trimmedLines[4]));
    subMain->inHandA->move(load_aspect(trimmedLines[1]),load_aspect(trimmedLines[2]));
    subMain->inHandA->rotate(load_aspect(trimmedLines[3]));
    for(int i = 0;i < subMain->station->modules.size();i++)
    {
        if(lp_point(subMain->inHandA->loc,*subMain->station->modules[i]->hit))//inHand is inside
        {
            subMain->station->modules[i]->contents = subMain->inHandA;
            subMain->inHandA->inside = subMain->station->modules[i];
            subMain->station->teams.push_back(subMain->inHandA);
            for(int j = 0;j < subMain->inHandA->members.size();j++)
            {
                subMain->inHandA->members[j]->inside = subMain->station->modules[i];
            }
        }
    }
    subMain->inHandA = NULL;//place astronaut
}

void GameState::un_load_save()
{
    money = 0;
    timeAccel = 0;
    for(int i = 0;i < subMain->stations.size();i++)
    {
        delete subMain->stations[i];
    }
    subMain->stations.clear();
    subMain->station = NULL;
    subMain->inHandA = NULL;
    subMain->inHand.clear();
    subMain->selected.clear();
    subMain->inHandOffsets.clear();
    subMain->add_overlay(OverlayType::INFOBANNER);
    subMain->add_overlay(OverlayType::EDITOR);
    for(int i = 0;i < subResearch->techOptions.size();i++)
    {
        subResearch->techOptions[i]->completion = 0;
    }
    subResearch->techOptions[0]->completion = 100;
    subResearch->queue.clear();
    for(int i = 0;i < subResearch->techOptions.size();i++)
    {
        subResearch->techOptions[i]->in_queue = false;
    }
    for(int i = 0;i < subMission->missions.size();i++)
    {
        subMission->missions[i]->completion = false;
    }
}





























