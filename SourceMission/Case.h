#ifndef CASE_H
#define CASE_H

class Case
{
public:
    virtual bool eval() = 0;
};

class Case3Astros: public Case
{
public:
    bool eval();
}extern case3Astros;

#endif // CASE_H

