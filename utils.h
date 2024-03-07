#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <math.h>

double distance(int p1X, int p1Y, int p2X, int p2Y) {
    int dx = p2X - p1X;
    int dy = p2Y - p1Y;
    return std::sqrt(dx * dx + dy * dy);
}

int getRandomNumber(int l, int r) {
    return l + rand() % (r - l + 1);
}

#endif