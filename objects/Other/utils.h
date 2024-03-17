#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <math.h>

double distance(const int &p1X, const int &p1Y, const int &p2X, const int &p2Y);
int getRandomNumber(const int &l, const int &r);
bool checkCollision(const int &p1X, const int &p1Y, const int &p1W, const int &p1H, const int &p2X, const int &p2Y, const int &p2W, const int &p2H);
void setPosToBorderPos(int &p1X, int &p1Y, const int &p1W, const int &p1H, const int &p2X, const int &p2Y, const int &p2W, const int &p2H);
void calculateVelocityToMouse(int &x, int &y, double speed);

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

void setPosToBorderPos(int &p1X, int &p1Y, const int &p1W, const int &p1H, const int &p2X, const int &p2Y, const int &p2W, const int &p2H)
{
    if (p1X > p2X + p2W) p1X = p2X + p2W;
    if (p1X + p1W < p2X) p1X = p2X - p1W;

    if (p1Y > p2Y + p2H) p1Y = p2Y + p2H;
    if (p1Y + p1H < p2Y) p1Y = p2Y - p1H;
}

void calculateVelocityToMouse(int &x, int &y, double speed)
{
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    double dx = mouseX - x;
    double dy = mouseY - y;

    double length = distance(x, y, mouseX, mouseY);
    if (length != 0) {
        dx /= length;
        dy /= length;
    }

    // Scale direction vector by speed to get velocity vector
    x = (dx * speed) + trunc(2 * (double)(SCREEN_HEIGHT - mouseY) / SCREEN_HEIGHT);
    y = dy * speed;
}

#endif