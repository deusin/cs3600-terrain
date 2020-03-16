#pragma once

#include <cmath>


class Rat
{
public:
    Rat(double x, double y, double degrees);
    void Draw();
    void SpinLeft();
    void SpinRight();
    void ScurryForward();

    double GetX() { return x; }
    double GetY() { return y; }
    double GetDegrees() { return degrees; }
    double GetRadius() { return radius; }
    double GetDX() { return std::cos(degrees * 3.1415926 / 180.); }
    double GetDY() { return std::sin(degrees * 3.1415926 / 180.); }
    double GetNextX() { return x + GetDX() * speed; }
    double GetNextY() { return y + GetDY() * speed; }


    void SetX(double newX) { x = newX; }
    void SetY(double newY) { y = newY; }

private:
    double x, y;
    double degrees;
    double radius;
    double speed;
    double turnspeed;
};

