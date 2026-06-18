#pragma once

#include <QWidget>
#include <QPointF>
#include <memory>

#include "document.h"

enum class Tool {
    Select,
    DrawPoint,
    DrawLine,
    DrawCircle
};

/*
    Класс хоолст для рисования
*/

class CadWidget : public QWidget {

public:
    CadWidget(Document* doc, QWidget* parent = nullptr);
    
    void setTool(Tool tool);
    
public slots:
    void saveToFile();
    void loadFromFile();
    void newDocument();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override; 

private:
    Document* document_;
    
    Tool currentTool_ = Tool::Select;
    int selectedId_ = -1;
    
    bool drawingInProgress_ = false;
    double firstClickX_ = 0, firstClickY_ = 0;
    double mouseX_ = 0, mouseY_ = 0;
    int nextId_ = 100;
    
    void selectShapeAt(double x, double y);
    double distanceToLine(double x, double y, double x1, double y1, double x2, double y2);
    void addPointHere(double x, double y);
    void addLineHere(double x1, double y1, double x2, double y2);
    void addCircleHere(double cx, double cy, double radius);    
};