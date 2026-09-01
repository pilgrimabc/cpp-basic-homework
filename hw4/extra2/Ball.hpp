#pragma once
#include "Painter.hpp"
#include "Point.hpp"
#include "Velocity.hpp"

class Ball {
public:
    void setVelocity(const Velocity& velocity);
    Velocity getVelocity() const;
    void draw(Painter& painter) const;
    void setCenter(const Point& center);
    Point getCenter() const;
    double getRadius() const;
    double getMass() const;
    bool isCollidable() const;

    Ball() = default;

    Ball(const Velocity& velocity, const Point& center, double radius, const Color& color, bool collible);

private:
    Velocity velocity; 
    Point center;
    double radius;
    Color color;
    bool collidable;
};
