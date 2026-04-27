#pragma once
#include "Shape.h"
#include "Point.h"
#include <string>
#include <sstream>

/*
    Класс наследник Линия
    Состоит из двух точек (класс Point)
*/

class Line : public Shape
{
    public:
        Line(int id, double x1, double y1, double x2, double y2) : Shape(id), start_(x1, y1), end_(x2, y2) {}
        
        std::string getInfo() const override{
            std::stringstream oss;
            oss << "Line [id="<<getId()<<"]: ("<<getStartX()<<", "<<getStartY()<<") -> ("<<getEndX()<<", "<<getEndY()<<")";
            return oss.str();
        }

        double getArea() const override {
            return 0;
        }

        double getStartX() const {return start_.getX();}
        double getStartY() const {return start_.getY();}
        double getEndX() const {return end_.getX();}
        double getEndY() const {return end_.getY();}

    private:
        Point start_;
        Point end_;
};