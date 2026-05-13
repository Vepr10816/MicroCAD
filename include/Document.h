#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "Shape.h"
#include <iostream>
#include <string>

class Document
{
public:
    void addShape(std::unique_ptr<Shape> shape) {
        shapes_.push_back(std::move(shape));
    }

    void printAll() const {
        std::cout << "=== Фигруры ===" << std::endl;
        for(auto& shape : shapes_) {
            std::cout << shape -> getInfo() << std::endl;
        }
    }

    Shape* findById(int id) const {
        auto it = findIterator(id);
        
        if(it != shapes_.end()) {
            return it->get();
        }
        return nullptr;
    }

    bool removeById(int id) {
        std::cout << "=== Удаление фигуры с ID=" << id << " ===" << std::endl;
        auto it = findIterator(id);

        if (it != shapes_.end()) {
            shapes_.erase(it);
            return true;
        }
        return false;
    }

    bool moveShape(int id, double dx, double dy) {
        Shape* shape = findById(id);
        std::cout << "=== Перемещение фигуры "<< shape -> getInfo() << " на (" << dx << ", " << dy << ") ===" << std::endl; 
        if(shape) {
            shape->move(dx, dy);
            return true;
        }
        return false;
    }

    bool scaleShape(int id, double factor) {
        Shape* shape = findById(id);
        std::cout << "=== Масштабирование фигуры "<< shape -> getInfo() << " с коэффициентом "  << factor << " ===" << std::endl; 
        if(shape) {
            shape -> scale(factor);
            return true;
        }
        return false;
    }

    void printBoundingBox() const {
        if (shapes_.empty()) {
            std::cout << "Нет фигур в документе." << std::endl;
            return;
        }

        BoundingBox total = shapes_[0]->getBounds();

        for(size_t i = 1; i < shapes_.size(); i++) {
            BoundingBox current = shapes_[i]->getBounds();
            if(current.minX < total.minX) total.minX = current.minX;
            if(current.minY < total.minY) total.minX = current.minY;
            if(current.maxX > total.maxX) total.maxX = current.maxX;
            if(current.maxY > total.maxY) total.maxY = current.maxY;
        }

        std::cout << "=== Охватывающий прямоугольник ===" << std::endl;
        std::cout << "Min: (" << total.minX << ", " << total.minY << ")" << std::endl;
        std::cout << "Max: (" << total.maxX << ", " << total.maxY << ")" << std::endl;
    }

private:
    std::vector<std::unique_ptr<Shape>> shapes_;

    std::vector<std::unique_ptr<Shape>>::const_iterator findIterator(int id) const {
        return std::find_if(shapes_.begin(), shapes_.end(),
            [id](const std::unique_ptr<Shape>& shape) {
                return shape->getId() == id;
            });
    }
};