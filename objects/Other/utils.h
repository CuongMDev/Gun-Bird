#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cmath>
#include <numeric>
#include <vector>

double distance(const int &p1X, const int &p1Y, const int &p2X, const int &p2Y);
int getRandomNumber(const int &l, const int &r);
bool checkCollision(const int &p1X, const int &p1Y, const int &p1W, const int &p1H, const int &p2X, const int &p2Y, const int &p2W, const int &p2H);
void setPosToBorderPos(int &p1X, int &p1Y, const int &p1W, const int &p1H, const int &p2X, const int &p2Y, const int &p2W, const int &p2H);
void calculateVelocityToMouse(int &x, int &y, double speed);
template<typename T>
T getRandomWithPercent(const std::vector<int> &percent, const std::vector<T> &type);

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

template<typename T>
T getRandomWithPercent(const std::vector<int> &percent, const std::vector<T> &type) {
    // Check if the vectors have the same size
    if (percent.size() != type.size()) {
        printf("Error: Vectors must have equal size");
        return 0;
    }


    int sum = std::accumulate(percent.begin(), percent.end(), 0);
    int randomNumber = getRandomNumber(1, sum);

    int preSum = 0,
            percentSize = (int)percent.size();
    for (int i = 0; i < percentSize; i++) {
        preSum += percent[i];
        if (randomNumber <= preSum) {
            return type[i];
        }
    }

    return type[0];
}

#endif