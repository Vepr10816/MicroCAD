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

    std::string getInfo() const override {
        std::stringstream oss;
        oss << "Point [id=" << getId() << "]: (" << x_ << ", " << y_ << ")";
        return oss.str();
    }

private:
    double x_;
    double y_;

};