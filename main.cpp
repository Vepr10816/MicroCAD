#include <iostream>
#include <vector>
#include <memory>
#include "point.h"
#include "line.h"
#include "circle.h"
#include "document.h"
#include <windows.h>


int main() {

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Document doc;
    doc.addShape(std::make_unique<Point>(1, 1, -2));
    doc.addShape(std::make_unique<Line>(2, 0, 0, 5, 6));
    doc.addShape(std::make_unique<Circle>(3, 10, 10, 4.5));

    doc.printAll();
    doc.printBoundingBox();

    std::cout << std::endl;

    if(doc.saveToFile("doc1.json")) {
        std::cout << "Успешное сохранение!" << std::endl;
    } else {
        std::cout << "Ошибка сохранения!" << std::endl;
    }

    std::cout << std::endl;

    Shape* found = doc.findById(2);
    if(found)
        found->move(1, 1);
    
    doc.moveShape(1, 3, 3);

    doc.removeById(1);

    doc.scaleShape(3, 10);

    doc.printAll();
    doc.printBoundingBox();

    std::cout << std::endl;

    if(doc.loadFromFile("doc1.json")) {
        std::cout << "Успешная загрузка!" << std::endl;
    } else {
        std::cout << "Ошибка загрузки!" << std::endl;
    }

    doc.printAll();
    doc.printBoundingBox();

    return 0;
}