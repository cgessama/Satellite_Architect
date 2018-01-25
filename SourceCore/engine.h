#ifndef ENGINE_H
#define ENGINE_H


#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <string>
#include "SOIL.h"
#include "SDL.h"
#include "SDL_opengl.h"


class Point;
class HitBox;
class Picture;
class Texture;
class Sprite;
class ArrayIndex;
class Font;

class Point
{
public:
    float x,y;
    Point() {}
    Point(float,float);
    Point operator + (Point);
    Point operator - (Point);
    Point operator * (double);
    void rotate(float);
    void render_p(Point*);
    float get_dist();//get_distance
};

class HitBox
{
    public:
    std::vector<Point> vert;
    Point orig;
    float rot;//total rotation of the Sprite
    HitBox();
    HitBox(float,float,float,float,float,float,float);//rotations preformed around given origin Point
    void fix();
    void rotate(float);
    void move(float,float);
    void scale(float);
    float find_radius();//find the furthest point from the origin and returns value
    float find_area();
    void render(float,float,float,float);
};

class Picture
{
public:
    std::string fileName;
    Uint32 texID;
    Picture();
    Picture(std::string);//no check here;
};

Picture * create_pic(std::string);

class Texture
{
public:
    HitBox * texCoords;
    virtual void render() = 0;
	virtual ~Texture(){};
};

class Image: public Texture
{
public:
    Picture * texData;
    Image();
    Image(float,float,float,float,float,std::string,float,float);
    Image(const Image *);
    ~Image();
    void render();
};

class Text
{
public:
    Font * font;
    HitBox * texCoords;
    std::string text;
    SDL_Color color;
    bool middle;
    std::vector<HitBox *> fontCoords;//where each letter is on the font texture
    std::vector<HitBox *> letterCoords;//where each letter is on the texture that will be generated and rendered
    float h;
    float w;//to prevent rounding errors
    Text();
    Text(float,float,float,float,std::string, Uint8, Uint8, Uint8,bool);
    void change_text(std::string);
    void render();
    ~Text();
};

class Animation: public Texture
{
public:
    std::vector<Picture *> frames;
    int current;
    Animation(float,float,float,float,float,std::string,float,float,int);
    void render();
	~Animation();
};

class ArrayIndex
{
public:
    ArrayIndex();
    bool stored;
    int index;
};

class Font
{
public:
    Font(std::string);
    std::vector<float> widths;
    std::vector<Point> locs;
    std::vector<HitBox *> letterCoords;
    float texW;
    float texH;
    float height;
    Picture * pic;
};

void render_rect(float,float,float,float,float,float,float,float);
int string_to_int(std::string);
float string_to_float(std::string);
std::string num_to_string(float);
int rInt(float);


#endif // ENGINE_H








