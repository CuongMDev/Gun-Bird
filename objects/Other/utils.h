#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include "../../Main/mainData.h"

double distance(const int &p1X, const int &p1Y, const int &p2X, const int &p2Y);
int getRandomNumber(const int &l, const int &r);
bool checkCollision(const int &p1X, const int &p1Y, const int &p1W, const int &p1H, const int &p2X, const int &p2Y, const int &p2W, const int &p2H);

void setPosWhenInternalCollision(int &p1X, int &p1Y, const int &p1W, const int &p1H, const int &p2X, const int &p2Y, const int &p2W, const int &p2H);
//change x1, y1 to velocityX1, velocityY1
void calculateVelocityBetweenTwoPos(int &x1, int &y1, const int &x2, const int &y2, const double &speed);

double angleBetweenTwoPos(const int &x1, const int &y1, const int &x2, const int &y2);

std::string formatTime(tm _time);

template<typename T>
T getRandomWithPercent(const std::vector<int> &percent, const std::vector<T> &type);

//-------------------------------------

double distance(const int &p1X, const int &p1Y, const int &p2X, const int &p2Y)
{
    int dx = p2X - p1X;
    int dy = p2Y - p1Y;
    return std::sqrt(dx * dx + dy * dy);
}

int getRandomNumber(const int &l, const int &r) {
    // Seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define a uniform distribution for integer numbers between l and r
    std::uniform_int_distribution<int> distribution(0, 1000);

    // Generate and return a random number
    return (distribution(gen) * distribution(gen) * distribution(gen) + distribution(gen) + distribution(gen) + distribution(gen)) % (r - l + 1) + l;
}

bool checkCollision(const int &p1X, const int &p1Y, const int &p1W, const int &p1H, const int &p2X, const int &p2Y, const int &p2W, const int &p2H)
{
    if (p1X > p2X + p2W) return false;
    if (p1X + p1W < p2X) return false;

    if (p1Y > p2Y + p2H) return false;
    if (p1Y + p1H < p2Y) return false;

    return true;
}

void setPosWhenInternalCollision(int &p1X, int &p1Y, const int &p1W, const int &p1H, const int &p2X, const int &p2Y, const int &p2W, const int &p2H)
{
    if (p1X > p2X + p2W) p1X = p2X + p2W;
    if (p1X + p1W < p2X) p1X = p2X - p1W;

    if (p1Y > p2Y + p2H) p1Y = p2Y + p2H;
    if (p1Y + p1H < p2Y) p1Y = p2Y - p1H;
}

void calculateVelocityBetweenTwoPos(int &x1, int &y1, const int &x2, const int &y2, const double &speed)
{
    double dx = x2 - x1;
    double dy = y2 - y1;

    double length = distance(x1, y1, x2, y2);
    if (length != 0) {
        dx /= length;
        dy /= length;
    }

    // Scale direction vector by speed to get velocity vector
    x1 = (dx * speed);// + trunc(2 * (double)(SCREEN_HEIGHT - y2) / SCREEN_HEIGHT);
    y1 = dy * speed;
}

double angleBetweenTwoPos(const int &x1, const int &y1, const int &x2, const int &y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    double angle = atan2(dy, dx) * 180 / M_PI;

    return angle;
}

std::string formatTime(tm _time)
{
    int hour = _time.tm_hour;   // Hour (0-23, adjust for 12-hour format if needed)
    int minute = _time.tm_min;   // Minute (0-59)
    int day = _time.tm_mday;          // Day of the month (1-31)
    int month = _time.tm_mon + 1;    // Month (0-indexed, January = 0)
    int year = _time.tm_year + 1900;  // Year since 1900

    return (hour < 10 ? "0" : "") + std::to_string(hour) + ':' + (minute < 10 ? "0" : "") + std::to_string(minute) + '/'
           + std::to_string(day) + '-' + std::to_string(month) + '-' + std::to_string(year);
}

template<typename T>
T getRandomWithPercent(const std::vector<int> &percent, const std::vector<T> &type) {
    // Check if the vectors have the same size
    if (percent.size() != type.size()) {
        printf("Error: Vectors must have equal size %d != %d", percent.size(), type.size());
        return type[0];
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