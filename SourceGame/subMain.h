#ifndef SUBMAIN_H
#define SUBMAIN_H

#include "SourceGame/subState.h"
#include "SourceCore/engine.h"

class SubState;
class Astronaut;
class Module;
class Overlay;
class Station;
class Team;
class Stage;


class SubMain: public SubState
{
public:
    Point gameMousePos;

    void change_station(int);
    Station * station;
    std::vector<Station *> stations;

    bool showOverlays;
    std::vector<int> previouslyActiveOverlays;//the overlays that were active before f1 was pressed
    std::vector<Overlay *> overlays;//----------------------------------------------------------------------------UI
    std::vector<Overlay *> activeOverlays;
    std::vector<Module *> inHand;
    std::vector<Module *> selected;
    std::vector<Point> inHandOffsets;
    bool noPlaceBool;

    Team * inHandA;

    const float windowRotSpeed = 1;//---------------------------------------------------coreConsts
    const float inHandRotSpeed = 2;
    const float windowMoveSpeed = 5;
    float inHandTechRot;

    SubMain();
    void handle_events();
    void logic();
    void render();

    void handle_key_up();//---------------------------------------------------------------------Events
    void handle_click();
    void key_logic();
    void toggle_overlay(int);
    int find_overlay(int);
    void add_overlay(int);
    void overlay_to_front(int);

    void calc_mouse_pos();//--------------------------------------------------------------------Core Logic
    void inhand_logic();

    //void handle_pickup();//picking up a module //-----------------------------------------------inHand core events
    void handle_locking();
    void handle_placement();
    void add_inHand(Module *);
    void rotate_inHand(float);
    void delete_inHand();
	void destroy_selection();
    void handle_no_place();
    void render_no_place();
    void update_window();

    std::vector<std::string> toolTip;
    std::vector<std::string> popups;
    std::vector<Uint32> popups_time;
    void show_tool_tip();
    void create_popups(std::string);
    void show_popups();

};

extern SubMain * subMain;

#endif // SUBMAIN_H
