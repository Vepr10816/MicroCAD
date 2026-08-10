#include <iostream>
#include <vector>
#include <memory>
#include "point.h"
#include "line.h"
#include "circle.h"
#include "document.h"
#include <windows.h>

//Обработка некорректных значений
double getValue() {
    double value;
    while (true) {
        std::cin >> value;
        if (!std::cin.fail()) {
            return value;
        }
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Введите корректное значение." << std::endl;
    }
}

int main() {

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Document doc;
    doc.addShape(std::make_unique<Point>(1, 1, -2));
    doc.addShape(std::make_unique<Line>(2, 0, 0, 5, 6));
    doc.addShape(std::make_unique<Circle>(3, 10, 10, 4.5));



    while(true) {
        std::cout << std::endl;
        doc.printAll();
        std::cout << std::endl;
        std::cout << "Введите id фигуры (0 для выхода):" << std::endl;
        int id;
        std::cin >> id;

        //Обработка некорректных без функции, так как int
        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Введите корректное значение от 0 до " + std::to_string(doc.size()) << std::endl;
            continue;
        }

        if(id == 0)
            break;

        //Есть метод findById, но продемонстрирую простую реализацию тут
        //Реализация поиска
        Shape* shape = nullptr;
        for(const auto& shapePtr : doc.getShapes()) {
            if(shapePtr->getId() == id) {
                shape = shapePtr.get();
                break;
            }
        }
        if(!shape) {
            std::cout << "Фигура не найдена" << std::endl;
            continue;
        }

        //Редактирование в зависимости от выбранной фигуры при помощи dynamic_cast
        //Точка
        if(Point* p = dynamic_cast<Point*>(shape)) {
            std::cout << "Выбранная фигура Точка.\nВведите координату x:" << std::endl;
            double x = getValue();
            std::cout << "Введите координату y:" << std::endl;
            double y = getValue();

            p->move(x - p->getX(), y - p->getY());
            std::cout << p->getInfo() << std::endl;

            continue;
        } 
        //Круг
        else if(Circle* c = dynamic_cast<Circle*>(shape)) {
            std::cout << "Выбранная фигура Круг.\nРедактирование центра.\n Введите координату x:" << std::endl;
            double x = getValue();
            std::cout << "Введите координату y:";
            double y = getValue();

            std::cout << "Редактирование радиуса.\n Введите радиус:" << std::endl;
            double radius = getValue();

            //Использую уже написаные методы (Можно проще - создать сеттеры, но думаю суть задания не в этом)
            if(radius > 0 && c->getRadius() > 0) {
                //scale - умножение всех координат фигуры на factor (Деление, чтобы получить коэффициент, на который нужно умножить старый радиус, чтобы получить введеный пользователем)
                c->scale(radius / c->getRadius());
            } else {
                std::cout << "Радиус  не изменен, введено некорретное значение" << std::endl;
            }
            //move - смещение фигуры (Вычитание чтобы поставить точку в веденные координаты)
            c->move(x - c->getCenterX(), y - c->getCenterY());

            std::cout << c->getInfo() << std::endl;
            continue;
        }
        //Линия. Тут через сеттеры, для демонстрации.
        else if(Line* l = dynamic_cast<Line*>(shape)){
            std::cout << "Выбранная фигура Линия.\nВведите координату X первой точки:" << std::endl;
            double x1 = getValue();
            std::cout << "Введите координату Y первой точки:" << std::endl;
            double y1 = getValue();
            std::cout << "Введите координату X второй точки:" << std::endl;
            double x2 = getValue();
            std::cout << "Введите координату Y второй точки:" << std::endl;
            double y2 = getValue();

            l->setStartXY(x1, y1);
            l->setEndXY(x2, y2);

            std::cout << l->getInfo() << std::endl;
            continue;
        }


    }
    return 0;
}