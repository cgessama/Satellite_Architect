#ifndef SUBSTATE_H
#define SUBSTATE_H

class SubState
{
    public:
    virtual void handle_events() = 0;
    virtual void logic() = 0;
    virtual void render() = 0;
};

#endif // SUBSTATE_H
