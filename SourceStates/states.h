#ifndef STATES_H
#define STATES_H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "SourceCore/engine.h"

class State
{
public:
    virtual void handle_events() = 0;
    virtual void logic() = 0;
    virtual void render() = 0;
};

extern State * active;

class Button
{
public:
    Text * tex;
    State * effect;
};

class KeyCom
{
public:
    int key;
    State * effect;
};

class MenuState: public State
{
public:
    Texture * bkg;
    std::vector<Button *> buttons;
    std::vector<KeyCom *> keyComs;
    MenuState();
    void handle_events();
    void logic();
    void render();
};
extern MenuState menuMain;
extern MenuState menuSettings;

class MenuLoad: public State
{
public:
    MenuLoad();
    const int fileTexesOffX = 75;
    const int fileTexesOffY = 75;
    const int fileTexesRelativeOffY = 50;
    Texture * bkg;
    std::vector<Button *> buttons;
    std::vector<KeyCom *> keyComs;
    std::vector<Text *> fileTexes;
    std::vector<std::string> fileNames;
    void handle_events();
    void logic();
    void render();
};
extern MenuLoad * menuLoad;

class GameState;

#endif // STATES_H






