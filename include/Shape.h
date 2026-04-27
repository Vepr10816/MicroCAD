#pragma once

#include <string>

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

    int getId() const {
        return id_;
    }

private:
    int id_;

};