#include "SourceCore/colide.h"
#include "SourceCore/engine.h"

bool lp_rect(HitBox obst,HitBox col)//obstacle and colider
{
    if(std::sqrt(std::pow(obst.orig.x - col.orig.x,2) + std::pow(obst.orig.y - col.orig.y,2)) < obst.find_radius() + col.find_radius())// if they are close enough to touch
    {
        for(int i = 0;i < obst.vert.size();i++)
        {
            for(int j = 0;j < col.vert.size();j++)
            {
                if(lp_point(col.vert[j] + col.orig,obst) == true)
                {
                    return true;
                }
                if(lp_point(obst.vert[i] + obst.orig,col) == true)
                {
                    return true;
                }
            }
        }
        if(lp_point(col.orig,obst) == true)//testing for origins
        {
            return true;
        }
        if(lp_point(obst.orig,col) == true)
        {
            return true;
        }
    }
    return false;
}

bool lp_point(Point dot,HitBox box)//preforms the linear programing for colision ( assumes origen is in side HitBox
{
    bool temp_b = true;
    bool orig_state = false;// is the origin greater or less than
    for(int i = 0;i < box.vert.size();i++)
    {
        orig_state = lin_inequal(ymxb(box.vert[i],box.vert[(i + 1) % box.vert.size()]),Point(0,0));//find origin State detirmined in local space for simplicity
        if(lin_inequal(ymxb(box.vert[i] + box.orig,box.vert[(i + 1) % box.vert.size()] + box.orig),dot) != orig_state)
        {
            temp_b = false;
            break;
        }
    }
    return temp_b;
}

bool lin_inequal(Point equa,Point dot)//equa hold m and b //returns true if Point is greater than given line
{
    if(dot.y > (equa.x * dot.x) + equa.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Point ymxb(Point a,Point b)//returns slope and y-int of given points
{
    Point temp;
    temp.x = (b.y - a.y) / (b.x - a.x);
    temp.y = a.y - (temp.x * a.x);
    return temp;
}


