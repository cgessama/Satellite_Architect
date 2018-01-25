#ifndef COLIDE_H
#define COLIDE_H

class Point;
class HitBox;

bool lp_rect(HitBox,HitBox);

bool lp_point(Point,HitBox);

Point ymxb(Point,Point);

bool lin_inequal(Point,Point);

#endif // COLIDE_H
