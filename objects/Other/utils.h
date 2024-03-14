#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <math.h>

double distance(const int &p1X, const int &p1Y, const int &p2X, const int &p2Y)
{
    int dx = p2X - p1X;
    int dy = p2Y - p1Y;
    return std::sqrt(dx * dx + dy * dy);
}

int getRandomNumber(const int &l, const int &r) {
    return l + rand() % (r - l + 1);
}

bool checkCollision(const int &p1X, const int &p1Y, const int &p1W, const int &p1H, const int &p2X, const int &p2Y, const int &p2W, const int &p2H)
{
    if (p1X > p2X + p2W) return false;
    if (p1X + p1W < p2X) return false;

    if (p1Y > p2Y + p2H) return false;
    if (p1Y + p1H < p2Y) return false;

    return true;
}

#endif