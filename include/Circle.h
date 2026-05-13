#pragma once

#include <string>
#include <sstream>
#include <numbers>
#include "Shape.h"
#include "Point.h"

/*
    Класс наследник Круг
    Центр из двух точек (класс Point)
    Радиус должен быть положительным, поэтому выставлено минимальное значение, чтобы не ломать алгоритмы
*/

class Circle : public Shape
{
public:
    Circle(int id, double centreX, double centreY, double radius) : Shape(id), centre_(centreX, centreY), radius_(radius) {
        if(radius_ <= 0)
            radius_ = 0.001;
    }

    std::string getInfo() const override {
        std::stringstream oss;
        oss << "Circle [id="<<getId()<<"]: centre=("<<centre_.getX()<<", "<<centre_.getY()<<"), radius="<<radius_<<"";
        return oss.str();
    }

    double getArea() const override {
        return std::numbers::pi * (radius_ * radius_);
    }

    void move(double dx, double dy) override {
        centre_.move(dx, dy);
    }

    void scale(double factor) {
        centre_.scale(factor);
        radius_ *= factor;
    }

    BoundingBox getBounds() const override {
        return BoundingBox{
            getCenterX() - getRadius(), getCenterY() - getRadius(),
            getCenterX() + getRadius(), getCenterY() + getRadius(),
        };
    } 

    double getCenterX() const {return centre_.getX();}
    double getCenterY() const {return centre_.getY();}
    double getRadius() const {return radius_;}

private:
    Point centre_;
    double radius_;
};