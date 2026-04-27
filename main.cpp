#include <iostream>
#include <vector>
#include <memory>
#include "Point.h"
#include "Line.h"
#include "Circle.h"


int main() {

    std::vector <std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Point>(1, 2.22, -1.2));
    shapes.push_back(std::make_unique<Line>(2, 0, 2, 7, 9));
    shapes.push_back(std::make_unique<Circle>(3, 0, 0, 25));

    std::cout << "Количество фигур " << shapes.size() << ": " << std::endl << std::endl;

    for(const auto& shape : shapes) {
        std::cout << shape->getInfo() << ", Area = " << shape->getArea() << std::endl;
    }

    return 0;
}