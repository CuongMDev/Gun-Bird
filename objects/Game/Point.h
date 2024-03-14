#ifndef POINT_H
#define POINT_H

#include "../Other/Object.h"

const int pointPosX = 10;
const int pointPosY = 10;

class Point : public Object
{
    void loadFont();

public:
    Point();
    ~Point();

    void init();
};

Point::Point()
{
    init();
}

Point::~Point()
= default;


#endif //POINT_H
