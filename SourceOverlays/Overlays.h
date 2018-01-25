#ifndef Overlays_H
#define Overlays_H

#include "SourceGame/subMain.h"
#include "SourceStates/game_state.h"
#include "SourceOverlays/enums.h"

class TechOption;

class Overlay
{
public:
    Overlay();
    Texture * bkg;
    std::vector<int> excludes;//UI type that cant be on at the same time
    std::vector<HitBox *> hits;
    std::vector<bool> hitsInclusive;//if the above hit boxes are inclusive or exclusive
    bool pickedUp;//used in overlays that move and for UI scaling
    int type;
    void base_render();
    virtual void change(int){};
    virtual float get_val(int){return 0;};
    virtual bool handle_click(){return true;};//returns true it the events fxn should continue
    virtual bool handle_key(){return true;};
    virtual void logic(){};
    virtual void render(){base_render();};
};

class OverlayEditor : public Overlay
{
public:
    OverlayEditor();
    std::vector< std::vector<Texture *> > Buttons;
    std::vector< std::vector<int> > prices;
    std::vector< std::vector<TechOption *> >techLevels;
    const int buttonDim = 100;
    const int buttonOffY = 70;//distance from bottom
    const int buttonOffX = 129;//distance from each other
    const int buttonOrigOffX = 85;//original x offset
    const int buttonTextOffY = 155;//text distance from bottom
    Text * name;//says Editor in bottom right corner
    Text * lockingTex;
    bool locking;
	void change(int);
    float get_val(int);
    bool handle_click();
    bool handle_key();
    void logic();
    void render();
    void init_stage0();
    void init_stage1();
};

class OverlayManager : public Overlay
{
public:
    OverlayManager();
    Text * sizeText;
    Texture * plusTex;
    Texture * minusTex;
    int teamSize;
    std::vector<Texture *> Buttons;
    std::vector<Text *> ButtonsText;
    const int buttonDim = 100;
    const int buttonOffY = 70;//distance from bottom
    const int buttonOffX = 60;//x offset
    Text * nameTex;
    bool handle_click();
    bool handle_key();
    void logic();
    void render();
};

class OverlayDetailPanel : public Overlay
{
public:
    OverlayDetailPanel(int,float,float,SDL_Color,SDL_Color,std::string,int);
    Text * nameTex;
    Point loc;//overall location
    Point veiwSpaceLoc;
    Point veiwSpaceDim;
    Point offset;
    void render();
    void change(int);
    void move3(float,float);
    bool handle_click();
    void logic();
    int detail;
    float lowest_val;
    float highest_val;
    SDL_Color low_c;
    SDL_Color high_c;
};

class OverlayInfoBanner : public Overlay
{
public:
    OverlayInfoBanner();
    std::vector<std::vector<Text *> > buttons;
    std::vector<HitBox *> timeHits;
    const int statsBeginY = 5;
    const int statsTextH = 50;
    const int moneyBeginX = 300;
    const int moneyNumBeginX = moneyBeginX + 180;
    const int techBeginX = 700;
    const int techNumBeginX = techBeginX + 150;
    Text * moneyTex;
    Text * techTex;
    Text * moneyNumTex;
    Text * TechNumTex;
    Text * rTex;
    Text * mTex;
    Texture * playTex;
    Texture * pauseTex;
    Texture * leaf;
    Texture * end_leaf;
    const int pauseHitXBegin = 1175;
    const int timeHitsXBegin = 1224;
    const int timeHitsXOffset = 44;
    const int timeHitsYBegin = 8;
    const int timeHitsDim = 45;
    void change(int);
    void logic();
    bool handle_click();
    void render();
};

class OverlayExitMenu : public Overlay
{
public:
    OverlayExitMenu();
    Text * nameTex;
    std::vector<Text *> buttons;
    void render();
    bool handle_click();
};

class OverlayEnergyInfo : public Overlay
{
public:
    OverlayEnergyInfo();
    Text * nameTex;

    Text * storeTex;
    Text * produceTex;
    Text * requestTex;
    Text * consumeTex;
    Text * deficitTex;
    Text * netTex;

    Text * storeNumTex;
    Text * produceNumTex;
    Text * requestNumTex;
    Text * consumeNumTex;
    Text * deficitNumTex;
    Text * netNumTex;

    int oldStored;
    int oldStorage;
    int oldProduce;
    int oldRequest;// just leave these
    int oldConsume;
    int oldDeficit;
    int oldNet;

    Point loc;
    Point offset;//for moving(offset of mouse pointer)
    void render();
    void logic();
    void move(float,float);
    bool handle_click();
    void update_texes();
};
#endif // Overlays_H






























