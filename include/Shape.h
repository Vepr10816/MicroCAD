#pragma once

#include <string>
#include "bounding_box.h"
#include "json.hpp"

/*
    Абстрактный класс Фигура
    Конструктор по умолчанию - для точек, которые не являются фигурой. (Например точка входящая в линию)
*/

class Shape {

public:
    Shape(int id) : id_(id) {}
    Shape() : id_(-1) {}

    virtual ~Shape() = default;

    virtual std::string getInfo() const = 0;

    virtual double getArea() const = 0;

    virtual void move(double dx, double dy) = 0;

    virtual void scale(double factor) = 0;

    virtual BoundingBox getBounds() const = 0;

    virtual nlohmann::json toJson() const = 0;

    virtual void fromJson(const nlohmann::json& json) = 0;

    int getId() const {
        return id_;
    }
    void setId(int id) {
        id_ = id;
    }

private:
    int id_;

};