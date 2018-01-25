#include "SourceModule/modules.h"
#include "SourceAstronauts/astronaut.h"
#include "SourceStations/stations.h"
#include "SourceStates/game_state.h"
#include "SourceAstronauts/team.h"
#include "SourceModule/detail_enums.h"
#include "SourceCore/colide.h"
#include "SourceModule/mod_enums.h"


Module::Module()
{
    type = -1;
    income = .1;
    capacity = 1;
    inHandi = new ArrayIndex;
    selectedi = new ArrayIndex;
    modulesi = new ArrayIndex;
    contents = NULL;
    if(subMain != NULL)//for modules there by default (created in the gameMain constructor
    {
        modulesi->stored = true;
        modulesi->index = subMain->station->modules.size();//no -1 because it is before it is officially added
        subMain->station->modules.push_back(this);
        station = subMain->station;
    }
    details.push_back(new Detail(0,10));
    details.push_back(new Detail(0,10));
    details.push_back(new Detail(1,0));//lines up with DetailType
    energyProduction = 0;
    energyConsumtion = 0;
    energyStorage = 0;
    energyStored = 0.01;
    volume = true;
}

void new_module(int type)
{
    subMain->inHandTechRot = subMain->station->gameSpace->rot;
    subMain->inHandOffsets.push_back(Point(0,0));
    switch(type)
    {
	case ModuleType::BASIC:
        subMain->inHand.push_back(new BasicModule());
        break;
	case ModuleType::HEATER:
        subMain->inHand.push_back(new HeaterModule());
        break;
	case ModuleType::OXYTANK:
        subMain->inHand.push_back(new OxygenTankModule());
        break;
	case ModuleType::BATTERY:
        subMain->inHand.push_back(new BatteryModule());
        break;
	case ModuleType::ADVANCED:
        subMain->inHand.push_back(new AdvancedModule());
        break;
	case ModuleType::AIRLOCK:
        subMain->inHand.push_back(new AirlockModule());
        break;
	case ModuleType::OXYSYNTH:
        subMain->inHand.push_back(new OxygenSynthModule());
        break;
	case ModuleType::SOLAR:
        subMain->inHand.push_back(new SolarPanelModule());
        break;
	case ModuleType::VAB:
        subMain->inHand.push_back(new VABModule());
        break;
	case ModuleType::LAUNCHPAD:
        subMain->inHand.push_back(new LaunchPadModule());
        break;
	case ModuleType::RANDD:
        subMain->inHand.push_back(new RandDModule());
        break;
	case ModuleType::COMS:
		subMain->inHand.push_back(new ComsModule());
		break;
	case ModuleType::HEX:
		subMain->inHand.push_back(new HexModule());
		break;
	case ModuleType::BIO:
		subMain->inHand.push_back(new BioModule());
		break;
    }
    subMain->inHand[subMain->inHand.size() - 1]->inHandi->stored = true;
    subMain->inHand[subMain->inHand.size() - 1]->inHandi->index = subMain->inHand.size() - 1;
    subMain->inHand[subMain->inHand.size() - 1]->type = type;
}


void Module::get_energy()
{
    station->energyProduced += energyProduction;
    station->energyRequested += energyConsumtion;
    station->energyStored += energyStored;
    station->totalEnergyStorage += energyStorage;
}

void Module::send_energy()
{
    station->energyConsumed += energyConsumtion * station->energyFactor;
    if(energyStored == 0)
    {
        energyStored = 0.01;
    }
    energyStored *= station->energyStorageFactor;
    if(energyStored > energyStorage)
    {
        energyStored = energyStorage;
    }
}

void Module::base_logic()
{
    if(lp_point(subMain->gameMousePos,*hit) && inHandi->stored == false)
    {
        mouse_over();
    }
    if(volume)
    {
        if(station->stage_type == 0)
        {
            details[DetailType::OXYGEN]     ->value += (subMain->station->stage->default_oxy  - details[DetailType::OXYGEN]     ->value) * ((1 - details[DetailType::INTEGRITY]->value) / 100) * gameMain->timeAccel;
            details[DetailType::TEMPERATURE]->value += (subMain->station->stage->default_temp - details[DetailType::TEMPERATURE]->value) * ((1 - details[DetailType::INTEGRITY]->value) / 100) * gameMain->timeAccel;
            details[DetailType::INTEGRITY]->value *= 1 - (gameMain->timeAccel / 100000.0);//not rounding issue
        }
        else
        {
            details[DetailType::OXYGEN]     ->value = subMain->station->stage->default_oxy;
            details[DetailType::TEMPERATURE]->value = subMain->station->stage->default_temp;
        }
    }
    for(int i = 0;i < nodes.size();i++)
    {
        if(nodes[i]->adjacent != NULL)
        {
            nodes[i]->logic();
        }
    }

}

void Module::mouse_over()
{
    if(volume && subMain->station->stage_type == 0)
    {
        subMain->toolTip.push_back("Oxygen: " + num_to_string(int(details[DetailType::OXYGEN]->value)) + "%");
        subMain->toolTip.push_back("Temperature: " + num_to_string(int(details[DetailType::TEMPERATURE]->value)) + " C");
        subMain->toolTip.push_back("Integrity: " + num_to_string(int(details[DetailType::INTEGRITY]->value * 100)) + " %");
    }
}

void Module::base_render()
{
    tex->render();
    if(subMain->inHand.size() != 0)
    {
        for(int i = 0;i < nodes.size();i++)
        {
            if(nodes[i]->adjacent == NULL)
            {
                nodes[i]->tex->texCoords->orig = nodes[i]->tex->texCoords->orig + tex->texCoords->orig;
                nodes[i]->tex->render();
                nodes[i]->tex->texCoords->orig = nodes[i]->tex->texCoords->orig - tex->texCoords->orig;
            }
        }
    }
    //hit->render(0,0,1,.5);
}

void Module::rotate(float r)
{
	//if you change something here remember to change in in space port
    tex->texCoords->rotate(r);
    hit->rotate(r);
    for(int i = 0;i < nodes.size();i++)
    {
        nodes[i]->tex->texCoords->orig.rotate(r);
    }
    if(contents != NULL)
    {
        contents->rotate(r);
    }
}

void Module::translate_m(float x,float y,bool to)//move to
{
    float tx,ty;//temps
    if(to)
    {
        tx = x - tex->texCoords->orig.x;
        ty = y - tex->texCoords->orig.y;
    }
    else
    {
        tx = x;
        ty = y;
    }
    tex->texCoords->move(tx,ty);
    hit->move(tx,ty);
    if(contents != NULL)
    {
        contents->move(tx,ty);
    }
}

void Module::draw_outline(float r,float g,float b,float a,float s)
{
    glBegin(GL_QUADS);
    glColor4f(r,g,b,a);
    for(int i = 0;i < hit->vert.size();i++)//4 because quads
    {
        glVertex2f(hit->vert[i].x * (1.0 + s) + hit->orig.x,hit->vert[i].y * (1.0 + s) + hit->orig.y);
        glVertex2f(hit->vert[i].x * (1.0 - s) + hit->orig.x,hit->vert[i].y * (1.0 - s) + hit->orig.y);
        glVertex2f(hit->vert[(i + 1) % hit->vert.size()].x * (1.0 - s) + hit->orig.x,hit->vert[(i + 1) % hit->vert.size()].y * (1.0 - s) + hit->orig.y);
        glVertex2f(hit->vert[(i + 1) % hit->vert.size()].x * (1.0 + s) + hit->orig.x,hit->vert[(i + 1) % hit->vert.size()].y * (1.0 + s) + hit->orig.y);
    }
    glEnd();
    glColor3f(1,1,1);
}

Module::~Module()
{
    delete tex;
    delete hit;
    for(int i  = 0;i< details.size();i++)
    {
        delete details[i];
    }
    details.clear();
    nodes.clear();
    for(int i = 0;i < station->modules.size();i++)
    {
        if(station->modules[i] == this)
        {
            station->modules.erase(station->modules.begin() + i);
        }
    }
    delete contents;
    nodes.clear();
    delete inHandi;
    delete selectedi;
    delete modulesi;
}

Detail::Detail(float v,float g)
{
    value = v;
    gradient = g;
}

Node::Node(float x,float y,Module * p)
{
    tex = new Image(-10,-10,20,20,0,"modules/node.png",x,y);
    rotation = 0;
    adjacent = NULL;
    connectedTo = NULL;
    parent = p;
}

void Node::connect(Node * n)
{
    connectedTo = n;
    connectedTo->connectedTo = this;
    adjacent = connectedTo->parent;
    connectedTo->adjacent = parent;
}

void Node::open()
{
    if(adjacent != NULL)
    {
        connectedTo->adjacent = NULL;
        connectedTo->connectedTo = NULL;
        connectedTo = NULL;
        adjacent = NULL;
    }
}

Point Node::get_dif(Node* n)
{
    return Point((parent->tex->texCoords->orig.x + tex->texCoords->orig.x) - (n->parent->tex->texCoords->orig.x + n->tex->texCoords->orig.x),
                 (parent->tex->texCoords->orig.y + tex->texCoords->orig.y) - (n->parent->tex->texCoords->orig.y + n->tex->texCoords->orig.y));
}

Node::~Node()
{
    open();
    delete tex;
}

void Node::logic()
{
    if(parent->volume && adjacent->volume && gameMain->timeAccel != 0)//verifiing volume
    {
        for(int i = 0;i < parent->details.size();i++)
        {
            if(parent->details[i]->gradient != 0)
            {
                float avg = (parent->details[i]->value + adjacent->details[i]->value) / 2;
                if(std::abs(parent->details[i]->value - avg) <
                   ((avg - parent->details[i]->value) / parent->details[i]->gradient) * gameMain->timeAccel)//gets rid of waffleing
                {
                    parent  ->details[i]->value = avg;
                    adjacent->details[i]->value = avg;
                }
                parent  ->details[i]->value += ((avg - parent  ->details[i]->value) / parent  ->details[i]->gradient) * gameMain->timeAccel;
                adjacent->details[i]->value += ((avg - adjacent->details[i]->value) / adjacent->details[i]->gradient) * gameMain->timeAccel;
            }
        }
    }
}

void Module::save(std::ofstream * file,int i)
{
    *file << "\t\t\t" << "Module: " << i << '\n';
    *file << "\t\t\t\t" << "Type: " << type << '\n';
    *file << "\t\t\t\t" << "Oxygen: " << details[DetailType::OXYGEN]->value << '\n';
    *file << "\t\t\t\t" << "Temperature: " << details[DetailType::TEMPERATURE]->value << '\n';
    *file << "\t\t\t\t" << "Integrity: " << details[DetailType::INTEGRITY]->value << '\n';
    *file << "\t\t\t\t" << "Energy Stored: " << energyStored << '\n';
    *file << "\t\t\t\t" << "Location X: " << tex->texCoords->orig.x << '\n';
    *file << "\t\t\t\t" << "Location Y: " << tex->texCoords->orig.y << '\n';
    *file << "\t\t\t\t" << "Rotation: " << tex->texCoords->rot << '\n';
    //station is saved else where
}







































