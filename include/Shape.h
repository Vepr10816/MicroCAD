#pragma once

#include <string>

/*
    Абстрактный класс Фигура
*/

class Shape {

public:
    Shape(int id) : id_(id) {}

    virtual ~Shape() = default;

    virtual std::string getInfo() const = 0;

    int getId() const {
        return id_;
    }

private:
    int id_;

};