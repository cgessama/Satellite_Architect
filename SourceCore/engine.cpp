#include "SourceCore/engine.h"
#include "SourceCore/consts.h"
#include "SourceCore/globals.h"

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

//vector
Point::Point(float xf,float yf)
{
    x = xf;
    y = yf;
}

void Point::rotate(float r)
{
    Point temp;
    temp.x = x;
    temp.y = y;
    x = (std::cos(r / 180.0 * PI) * temp.x) - (std::sin(r / 180.0 * PI) * temp.y);
    y = (std::sin(r / 180.0 * PI) * temp.x) + (std::cos(r / 180.0 * PI) * temp.y);

}

Point Point::operator + (Point v)
{
    return Point(x + v.x,y + v.y);
}

Point Point::operator - (Point v)
{
    return Point(x - v.x,y - v.y);
}

Point Point::operator * (double s)
{
    return Point(x * s,y * s);
}

void Point::render_p(Point * o)
{
    if(o == NULL)
    {
        o = new Point(0,0);
    }
    glBegin(GL_QUADS);
    glVertex2f(o->x + x - 5,o->y + y - 5);
    glVertex2f(o->x + x + 5,o->y + y - 5);
    glVertex2f(o->x + x + 5,o->y + y + 5);
    glVertex2f(o->x + x - 5,o->y + y + 5);
    glEnd();
}

float Point::get_dist()
{
    return std::sqrt(std::pow(x,2) + std::pow(y,2));
}



//HitBox
HitBox::HitBox()
{
    orig = Point(0,0);
    rot = 0;
}

HitBox::HitBox(float x,float y,float w,float h,float ox,float oy,float r)//rotations preformed around given origin Point
{
    orig.x = ox;
    orig.y = oy;
    vert.push_back(Point(x    ,y    ));
    vert.push_back(Point(x + w,y    ));
    vert.push_back(Point(x + w,y + h));
    vert.push_back(Point(x    ,y + h));
    rot = 0;
    fix();
}

void HitBox::fix()
{
    rotate(.01);//offset to avoid slopes and y ints of inf
    rot -= .01;
}

float HitBox::find_radius()
{
    float cur = 0;//stores the currently calculated radius
    float ret = 0;//val returned
    for(int i = 0;i < vert.size();i++)
    {
        cur = std::sqrt(std::pow(vert[i].x,2) + std::pow(vert[i].y,2));
        if(ret < cur)
        {
            ret = cur;
        }
    }
    return cur;
}

void HitBox::rotate(float r)
{
    rot += r;
    while(rot < 0 || rot > 360)
    {
        if(rot < 0)
        {
            rot += 360;
        }
        if(rot > 360)
        {
            rot -= 360;
        }
    }
    for(int i = 0;i < vert.size(); i++)
    {
        vert[i].rotate(r);
    }
}

void HitBox::move(float x,float y)
{
    orig.x += x;
    orig.y += y;
}

void HitBox::scale(float fac)
{
    for(int i = 0;i < vert.size();i++)
    {
        vert[i].x *= fac;
        vert[i].y *= fac;
    }
}

float HitBox::find_area()
{
    float a = 0;
    Point temp;
    float dist;
    for(int i = 0;i < vert.size();i++)
    {
        temp = Point((vert[i].x + vert[(i + 1) % vert.size()].x) / 2,(vert[i].y + vert[(i + 1) % vert.size()].y) / 2);
        dist = temp.get_dist();
        temp = vert[i] - vert[(i + 1) % vert.size()];
        a += dist * temp.get_dist() / 2;
    }
    return a;
}

void HitBox::render(float r,float g,float b,float a)
{
    glBegin(GL_TRIANGLES);
    glColor4f(r,g,b,a);
    for(int i = 0;i < vert.size();i++)
    {
        glVertex2f(orig.x + vert[i].x,orig.y + vert[i].y);
        glVertex2f(orig.x + vert[(i + 1) % vert.size()].x,orig.y + vert[(i + 1) % vert.size()].y);
        glVertex2f(orig.x,orig.y);
    }
    glEnd();
    glColor4f(1,1,1,1);
}




Picture::Picture()
{
    texID = 0;
    fileName = "";
}

Picture::Picture(std::string file)
{
    fileName = file;
    texID = SOIL_load_OGL_texture(file.c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,NULL);
    glBindTexture(GL_TEXTURE_2D,texID);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,NULL);
}

Picture * create_pic(std::string file)//this fxn is neccessary so that the consrutor isn't called in duplicate
{
    for(int i = 0;i < pics.size();i++)
    {
        if(pics[i] != NULL)//there is tex data here
        {
            if(pics[i]->fileName == file)
            {
                return pics[i];
            }
        }
    }
    for(int i = 0;i < pics.size();i++)
    {
        if(pics[i] == NULL)//finds empty spot
        {
            pics[i] = new Picture(file);
            return pics[i];
        }
    }
    pics.push_back(new Picture(file));
    return pics[pics.size() - 1];// -1 for block math
}




//tex
Image::Image()
{
    texData = NULL;
    texCoords = NULL;
}

Image::Image(float x,float y,float w,float h,float r,std::string file,float ox,float oy)
{
    texCoords = new HitBox(x,y,w,h,ox,oy,r);
    texData = create_pic(file);
}


//copy constructor
Image::Image(const Image *obj)
{
    texCoords = new HitBox(*obj->texCoords);
    texData = obj->texData;
}

Image::~Image()
{
    //texData is never deleted because this will not be a font and other stuff is stored in pics
    delete texCoords;
}

void Image::render()
{
    glBindTexture(GL_TEXTURE_2D,texData->texID);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(texCoords->orig.x + texCoords->vert[0].x,texCoords->orig.y + texCoords->vert[0].y);
    glTexCoord2f(1,0);
    glVertex2f(texCoords->orig.x + texCoords->vert[1].x,texCoords->orig.y + texCoords->vert[1].y);
    glTexCoord2f(1,1);
    glVertex2f(texCoords->orig.x + texCoords->vert[2].x,texCoords->orig.y + texCoords->vert[2].y);
    glTexCoord2f(0,1);
    glVertex2f(texCoords->orig.x + texCoords->vert[3].x,texCoords->orig.y + texCoords->vert[3].y);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,NULL);
}


Text::Text()
{
    color.r = 255;
    color.g = 0;
    color.b = 255;
    color.a = 255;
    text = "";
    texCoords = NULL;
    middle = false;
    font = dataFont;
    h = 0;
    w = 0;
}

Text::Text(float wf,float hf,float ox,float oy,std::string s, Uint8 r, Uint8 g, Uint8 b,bool m)
{
    font = dataFont;
    middle = m;
    if(middle)
    {
        texCoords = new HitBox(-wf / 2,-hf / 2,wf,hf,ox,oy,0);
    }
    else
    {
        texCoords = new HitBox(-.1,-.1,wf,hf,ox,oy,0);
    }
    h = hf;
    w = wf;
    change_text(s);
    color = {r,g,b,255};
}

void Text::change_text(std::string s)
{
    fontCoords.clear();//where each letter is on the font texture
    letterCoords.clear();//where each letter is on the texture that will be generated and rendered
    text = s;
    float totalWidths = 0;
    for(int i = 0;i < s.size();i++)
    {
        fontCoords.push_back(font->letterCoords[s[i] - 32]);
        totalWidths += font->widths[text[i] - 32];
    }
    float texAspectRatio = w / h;
    float surfAspectRatio = (totalWidths * font->texW) / (font->height * font->texH);
    Point loc = texCoords->orig;
    delete texCoords;
    texCoords = NULL;
    if(texAspectRatio > surfAspectRatio)//space is to wide so orient to hieight
    {
        if(middle)
        {
            texCoords = new HitBox(-h * surfAspectRatio / 2,-h / 2,h * surfAspectRatio,h,loc.x,loc.y,0);
        }
        else
        {
            texCoords = new HitBox(-.1,-.1,h * surfAspectRatio,h,loc.x,loc.y,0);
        }
    }
    else//to tall
    {
        if(middle)
        {
            texCoords = new HitBox(-w / 2,-w * (1 / surfAspectRatio) / 2,w,w * (1 / surfAspectRatio),loc.x,loc.y,0);
        }
        else
        {
            texCoords = new HitBox(-.1,-.1,w,w / surfAspectRatio,loc.x,loc.y,0);
        }
    }
    float texWidth  = texCoords->vert[1].x - texCoords->vert[0].x;
    float texHeight = texCoords->vert[3].y - texCoords->vert[0].y;
    float widthRatio = texWidth / totalWidths;
    float xLoc,yLoc;
    if(middle)
    {
        xLoc = -texWidth / 2;
        yLoc = -texHeight / 2;
    }
    else
    {
        xLoc = 0;
        yLoc = 0;
    }
    int charVal = 0;
    for(int i = 0;i < s.size();i++)
    {
        charVal = s[i] - 32;
        letterCoords.push_back(new HitBox(-.1,-.1,font->widths[charVal] * widthRatio,texHeight,xLoc,yLoc,0));
        xLoc += font->widths[charVal] * widthRatio;
    }
}

void Text::render()
{
    glBindTexture(GL_TEXTURE_2D,font->pic->texID);
    glBegin(GL_QUADS);
    glColor4f((float)color.r / 255,(float)color.g / 255,(float)color.b / 255,(float)color.a / 255);
    for(int i = 0;i < letterCoords.size();i++)
    {
        glTexCoord2f(  fontCoords[i]->orig.x +   fontCoords[i]->vert[0].x,  fontCoords[i]->orig.y +   fontCoords[i]->vert[0].y);
          glVertex2f(texCoords->orig.x + letterCoords[i]->orig.x + letterCoords[i]->vert[0].x,texCoords->orig.y + letterCoords[i]->orig.y + letterCoords[i]->vert[0].y);
        glTexCoord2f(  fontCoords[i]->orig.x +   fontCoords[i]->vert[1].x,  fontCoords[i]->orig.y +   fontCoords[i]->vert[1].y);
          glVertex2f(texCoords->orig.x + letterCoords[i]->orig.x + letterCoords[i]->vert[1].x,texCoords->orig.y + letterCoords[i]->orig.y + letterCoords[i]->vert[1].y);
        glTexCoord2f(  fontCoords[i]->orig.x +   fontCoords[i]->vert[2].x,  fontCoords[i]->orig.y +   fontCoords[i]->vert[2].y);
          glVertex2f(texCoords->orig.x + letterCoords[i]->orig.x + letterCoords[i]->vert[2].x,texCoords->orig.y + letterCoords[i]->orig.y + letterCoords[i]->vert[2].y);
        glTexCoord2f(  fontCoords[i]->orig.x +   fontCoords[i]->vert[3].x,  fontCoords[i]->orig.y +   fontCoords[i]->vert[3].y);
          glVertex2f(texCoords->orig.x + letterCoords[i]->orig.x + letterCoords[i]->vert[3].x,texCoords->orig.y + letterCoords[i]->orig.y + letterCoords[i]->vert[3].y);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D,NULL);
    glColor4f(1,1,1,1);
}

Text::~Text()
{
    delete texCoords;
}

//animation
Animation::Animation(float x,float y,float w,float h,float r,std::string file,float ox,float oy,int num)
{
    texCoords = new HitBox(x,y,w,h,ox,oy,r);
    current = 0;
    std::string s;
    for(int i = 0;i < num;i++)
    {
        s += file;
        s += "/";
        if(i < 9)
        {
            s += "0";
        }
        if(i < 99)
        {
            s += "0";
        }
        if(i < 999)
        {
            s += "0";
        }
        s += i + 1;
        s += ".png";
        frames.push_back(create_pic(s));
    }
}

void Animation::render()
{
    Image temp;
    temp.texData = frames[current];
    temp.texCoords = texCoords;
    temp.render();
    current++;
    if(current == frames.size())
    {
        current = 0;
    }
}

Animation::~Animation()
{
	delete texCoords;
}




//ArrayIndex
ArrayIndex::ArrayIndex()
{
    stored = false;
    index = 0;
}


Font::Font(std::string file)
{
    pic = create_pic("Fonts/" + file + ".png");
    glBindTexture(GL_TEXTURE_2D,pic->texID);
    int w,h;//because it requires ints
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
    texW = w;
    texH = h;
    glBindTexture(GL_TEXTURE_2D,NULL);
    std::ifstream widthsFile;
    std::string s;
    s += "Fonts/" + file + "Widths.txt";
    widthsFile.open(s.c_str());
    std::string line;
    std::string num;
    for(int i = 0;i < 95;i++)
    {
        std::getline(widthsFile,line);
        int j = 0;
        num = "";
        while(line[j] != ',')
        {
            num.push_back(line[j]);
            j++;
        }
        locs.push_back(Point(atoi(num.c_str()),0));
        num = "";
        j++;//avoid comma
        while(j < line.size())
        {
            num.push_back(line[j]);
            j++;
        }
        widths.push_back(atoi(num.c_str()));
    }
    std::getline(widthsFile,line);
    height = atoi(line.c_str());
    for(int i = 0;i < locs.size();i++)
    {
        if(i <= 30)
        {
            locs[i].y = 0;
        }
        else if(i <= 62)
        {
            locs[i].y = height;
        }
        else
        {
            locs[i].y = height * 2;
        }
    }
    height /= texH;
    for(int i = 0;i < locs.size();i++)
    {
        locs[i].x /= texW;
        locs[i].y /= texH;
        widths[i] /= texW;
    }
    for(int i = 0;i < locs.size();i++)//set up letterCoords
    {
        letterCoords.push_back(new HitBox(-.00001,-.00001,widths[i],height,locs[i].x,locs[i].y,0));
    }
}

//aux

void render_rect(float x,float y,float w,float h,float r,float g,float b,float a)
{
    glBegin(GL_QUADS);
    glColor4f(r,g,b,a);
    glVertex2f(x,y);
    glVertex2f(x + w,y);
    glVertex2f(x + w,y + h);
    glVertex2f(x,y + h);
    glEnd();
    glColor4f(1,1,1,1);
}

int string_to_int(std::string s)
{
    std::stringstream ss;
    ss.str(s);
    int ret;
    ss >> ret;
    return ret;
}

float string_to_float(std::string s)
{
    std::stringstream ss;
    ss.str(s);
    float ret;
    ss >> ret;
    return ret;
}

std::string num_to_string(float f)
{
    std::stringstream ss;
    ss << f;
    return ss.str();
}

int rInt(float f)
{
	return int(f + .5);
}






























