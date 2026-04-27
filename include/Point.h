#pragma once

#include "Shape.h"
#include <string>
#include <sstream>

/*
    Класс наследник Точка
*/

class Point : public Shape {

public:
    Point(int id, double x, double y) : Shape(id), x_(x), y_(y) {}
    Point(double x, double y) : x_(x), y_(y) {}

    std::string getInfo() const override {
        std::stringstream oss;
        oss << "Point [id=" << getId() << "]: (" << x_ << ", " << y_ << ")";
        return oss.str();
    }

    double getArea() const override{
        return 0;
    }

    double getX() const {return x_;}
    double getY() const {return y_;}

private:
    double x_;
    double y_;

};