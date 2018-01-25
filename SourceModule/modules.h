#ifndef MODULES_H
#define MODULES_H

#include <string>
#include <sstream>
#include "SourceCore/engine.h"

class Astronaut;
class Node;
class Detail;
class Station;
class Team;

class Module
{
public:
    Team * contents;
    Texture * tex;
    HitBox * hit;
    ArrayIndex * inHandi;
    ArrayIndex * modulesi;
    ArrayIndex * selectedi;
    std::vector<Detail *> details;
    std::vector<Node *> nodes;
    bool volume;
    float energyProduction;
    float energyConsumtion;
    float energyStorage;
    float energyStored;
    float capacity;//crew capacity
    int type;
    Station * station;
    float income;
    Module();
    virtual void get_energy();
    virtual void send_energy();
    virtual void mouse_over();
    void base_logic();
    void base_render();
    virtual void rotate(float);
    void translate_m(float,float,bool);
    void draw_outline(float,float,float,float,float);
    void save(std::ofstream *,int);
    virtual void logic() = 0;
    virtual void render() = 0;
    virtual ~Module();
};

class Node
{
public:
    Node(float,float,Module*);
    void open();
    void connect(Node*);
    Point get_dif(Node*);
    ~Node();
    float rotation;
    Node * connectedTo;
    Module * parent;
    Module * adjacent;
    Texture * tex;
    void logic();
};

class Detail
{
public:
    float value;
    float gradient;//per second
    Detail(float,float);
};


class BasicModule: public Module
{
public:
    BasicModule();
    void render();
    void logic();
};

class AdvancedModule: public Module
{
public:
    AdvancedModule();
    void render();
    void logic();
};

class HexModule : public Module
{
public:
	HexModule();
	void render();
	void logic();
};

class BioModule : public Module
{
public:
	BioModule();
	void render();
	void logic();
};

class AirlockModule: public Module
{
public:
    AirlockModule();
    void render();
    void logic();
};

class SolarPanelModule: public Module
{
public:
    SolarPanelModule();
    void render();
    void logic();
};

class HeaterModule: public Module
{
public:
    HeaterModule();
    void render();
    void logic();
    void get_energy();
    void send_energy();
    const float heatRate = 10;
    float energyRequested;
};

class OxygenSynthModule: public Module
{
public:
    OxygenSynthModule();
    void render();
    void logic();
    void get_energy();
    void send_energy();
    const float synthRate = 10;
    float energyRequested;
};

class OxygenTankModule: public Module
{
public:
    float storedOxy;
    OxygenTankModule();
    void get_energy();
    void send_energy();
    void render();
    void logic();
    const float flowRate = 10;
    const float start_level = 400;
    float energyRequested;
    void mouse_over();
};

class BatteryModule: public Module
{
public:
    BatteryModule();
    void render();
    void logic();
};

class VABModule: public Module
{
public:
    VABModule();
    void render();
    void logic();
};

class LaunchPadModule: public Module
{
public:
    LaunchPadModule();
    void render();
    void logic();
};

class RandDModule: public Module
{
public:
    RandDModule();
    void render();
    void logic();
};

class ComsModule : public Module
{
public:
	ComsModule();
	void render();
	void logic();
};

void new_module(int);

#endif // MODULES_H

















