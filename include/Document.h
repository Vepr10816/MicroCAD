#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>

#include "Shape.h"
#include "Point.h"    
#include "Line.h"     
#include "Circle.h"   

#ifdef _WIN32
#include <direct.h> // для mkdir
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h> // для mkdir на Linux/Mac
#define MKDIR(path) mkdir(path, 0777)
#endif

// Папка с сохранениями
constexpr const char* DATA_DIR = "data/";

// Проверка на существование
inline void ensureDirectoryExists() {
    MKDIR(DATA_DIR);
}

/*
    Класс композиция
    Управляет коллекцией фигур
    Отвечает за время жизни фигур. Когда Document уничтожается, умные указатели сами удалят все фигуры.
*/

class Document
{
public:

    const std::vector<std::unique_ptr<Shape>>& getShapes() const {
        return shapes_;
    }

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
        if(shape) {
            std::cout << "=== Перемещение фигуры "<< shape -> getInfo() << " на (" << dx << ", " << dy << ") ===" << std::endl;
            shape->move(dx, dy);
            return true;
        }
        return false;
    }

    bool scaleShape(int id, double factor) {
        Shape* shape = findById(id);
        if(shape) {
            std::cout << "=== Масштабирование фигуры "<< shape -> getInfo() << " с коэффициентом "  << factor << " ===" << std::endl; 
            shape -> scale(factor);
            return true;
        }
        return false;
    }

    BoundingBox getBoundingBox() const {
        if (shapes_.empty()) {
            return BoundingBox{0, 0, 0, 0};  // Пустой
        }

        BoundingBox total = shapes_[0]->getBounds();

        for (size_t i = 1; i < shapes_.size(); i++) {
            BoundingBox current = shapes_[i]->getBounds();
            if (current.minX < total.minX) total.minX = current.minX;
            if (current.minY < total.minY) total.minY = current.minY;  // ← Заметь: total.minY, а не total.minX!
            if (current.maxX > total.maxX) total.maxX = current.maxX;
            if (current.maxY > total.maxY) total.maxY = current.maxY;
        }

        return total;
    }
    
    void printBoundingBox() const {
        if (shapes_.empty()) {
            std::cout << "Нет фигур в документе." << std::endl;
            return;
        }

        BoundingBox bb = getBoundingBox();
        
        std::cout << "=== Охватывающий прямоугольник ===" << std::endl;
        std::cout << "Min: (" << bb.minX << ", " << bb.minY << ")" << std::endl;
        std::cout << "Max: (" << bb.maxX << ", " << bb.maxY << ")" << std::endl;
    }
    
    bool isEmpty() const {
        return shapes_.empty();
    }

    int size() const {
        return shapes_.size();
    }

    bool saveToFile(const std::string& fileName) const {

        ensureDirectoryExists(); 
        std::string fullPath = std::string(DATA_DIR) + fileName;

        nlohmann::json output;

        for(const auto& shape : shapes_) {
            output["shapes"].push_back(shape->toJson());
        }

        std::ofstream file(fullPath);
        if(!file.is_open()) {
            return false;
        }

        file << output.dump(4);
        return true;
    }

    bool loadFromFile(const std::string& fileName) {

        std::string fullPath = std::string(DATA_DIR) + fileName;

        std::ifstream file(fullPath);
        if(!file.is_open()) {
            return false;
        }

        nlohmann::json input;
        file >> input;

        shapes_.clear();

        for (const auto& item : input["shapes"]) {
            std::string type = item["type"];
            
            if (type == "point") {
                auto p = std::make_unique<Point>(0, 0, 0);
                p->fromJson(item);
                addShape(std::move(p));
            }
            else if (type == "line") {
                auto l = std::make_unique<Line>(0, 0, 0, 0, 0);
                l->fromJson(item);
                addShape(std::move(l));
            }
            else if (type == "circle") {
                auto c = std::make_unique<Circle>(0, 0, 0, 0);
                c->fromJson(item);
                addShape(std::move(c));
            }
        }
        
        return true;    
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