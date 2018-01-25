#ifndef SUBRESEARCH_H
#define SUBRESEARCH_H

#include "SourceGame/subState.h"
#include <vector>

class TechOption;
class Texture;
class Text;

class SubResearch: public SubState
{
public:
    Texture * bkg;
    Texture * queueHighlight;
    Text * queueNumber;
    Text * nameTex;
    Text * backTex;
    Text * techTex;
    bool intelCurrent;//if the intel tex is current
    std::vector <TechOption *> techOptions;
    std::vector <TechOption *> queue;
    SubResearch();
    void handle_events();
    void add_to_queue(TechOption *);
    void remove_from_queue(TechOption *);
    void fix_queue();
    void logic();
    void render();
};
extern SubResearch * subResearch;

#endif // SUBRESEARCH_H
