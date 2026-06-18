#pragma once

#include <string>
#include <sstream>
#include "shape.h"
#include "bounding_box.h"

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

    void move(double dx, double dy) override {
        x_ += dx;
        y_ += dy;
    }

    void scale(double factor) override {
        x_ *= factor;
        y_ *= factor;
    }

    BoundingBox getBounds() const override {
        return BoundingBox{x_, y_, x_, y_};
    }

    nlohmann::json toJson() const override {
        nlohmann::json json;
        json["type"] = "point";
        json["id"] = getId();
        json["x"] = x_;
        json["y"] = y_;

        return json;
    }

    void fromJson(const nlohmann::json& json) override {
        setId(json["id"].get<int>());
        x_ = json["x"].get<double>();
        y_ = json["y"].get<double>();
    }

    double getX() const {return x_;}
    double getY() const {return y_;}

private:
    double x_;
    double y_;

};