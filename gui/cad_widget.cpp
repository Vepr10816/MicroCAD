#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <cmath>
#include <iostream>

#include "cad_widget.h"

//Конструктор
CadWidget::CadWidget(Document* doc, QWidget* parent)
    : QWidget(parent), document_(doc) {
    setMinimumSize(800, 600);
    setWindowTitle("MicroCAD");
    setMouseTracking(true);
}

//Переключение фигур
void CadWidget::setTool(Tool tool) {
    currentTool_ = tool;
    drawingInProgress_ = false;
    update();
}

//Отрисовка фигур
void CadWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), Qt::white);
    
    // Рисуем все фигуры
    for (const auto& shapePtr : document_->getShapes()) {
        if (!shapePtr) continue;
        
        bool isSelected = (selectedId_ != -1 && shapePtr->getId() == selectedId_);
        
        if (Point* p = dynamic_cast<Point*>(shapePtr.get())) {
            painter.setPen(QPen(isSelected ? Qt::red : Qt::black, isSelected ? 8 : 6));
            painter.drawPoint(QPointF(p->getX(), p->getY()));
        }
        else if (Line* l = dynamic_cast<Line*>(shapePtr.get())) {
            painter.setPen(QPen(isSelected ? Qt::red : Qt::black, isSelected ? 3 : 2));
            painter.drawLine(QPointF(l->getStartX(), l->getStartY()),
                             QPointF(l->getEndX(), l->getEndY()));
        }
        else if (Circle* c = dynamic_cast<Circle*>(shapePtr.get())) {
            painter.setPen(QPen(isSelected ? Qt::red : Qt::black, 2));
            painter.setBrush(isSelected ? QColor(255, 0, 0, 40) : Qt::transparent);
            painter.drawEllipse(QPointF(c->getCenterX(), c->getCenterY()),
                                c->getRadius(), c->getRadius());
        }
    }
    
    // BoundingBox
    if (!document_->getShapes().empty()) {
        BoundingBox bb = document_->getBoundingBox();
        painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
        painter.setBrush(Qt::transparent);
        painter.drawRect(bb.minX, bb.minY, bb.maxX - bb.minX, bb.maxY - bb.minY);
    }
    
    // Предпросмотр
    if (drawingInProgress_ && currentTool_ != Tool::Select) {
        painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
        
        if (currentTool_ == Tool::DrawLine) {
            painter.drawLine(QPointF(firstClickX_, firstClickY_),
                             QPointF(mouseX_, mouseY_));
        }
        else if (currentTool_ == Tool::DrawCircle) {
            double radius = std::hypot(mouseX_ - firstClickX_, mouseY_ - firstClickY_);
            painter.drawEllipse(QPointF(firstClickX_, firstClickY_), radius, radius);
        }
    }
}

//Обработка кликов
void CadWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) return;
    
    double x = event->pos().x();
    double y = event->pos().y();
    
    switch (currentTool_) {
        case Tool::Select:
            selectShapeAt(x, y);
            break;
        case Tool::DrawPoint:
            addPointHere(x, y);
            break;
        case Tool::DrawLine:
            if (!drawingInProgress_) {
                firstClickX_ = x; firstClickY_ = y;
                drawingInProgress_ = true;
            } else {
                addLineHere(firstClickX_, firstClickY_, x, y);
                drawingInProgress_ = false;
            }
            break;
        case Tool::DrawCircle:
            if (!drawingInProgress_) {
                firstClickX_ = x; firstClickY_ = y;
                drawingInProgress_ = true;
            } else {
                double radius = std::hypot(x - firstClickX_, y - firstClickY_);
                addCircleHere(firstClickX_, firstClickY_, radius);
                drawingInProgress_ = false;
            }
            break;
    }
    update();
}

//Предпросмотр неотрисованной фигуры
void CadWidget::mouseMoveEvent(QMouseEvent* event) {
    mouseX_ = event->pos().x();
    mouseY_ = event->pos().y();
    if (drawingInProgress_) update();
}

void CadWidget::selectShapeAt(double x, double y) {
    selectedId_ = -1;
    for (const auto& shapePtr : document_->getShapes()) {
        if (!shapePtr) continue;
        
        if (Point* p = dynamic_cast<Point*>(shapePtr.get())) {
            if (std::hypot(x - p->getX(), y - p->getY()) < 10) {
                selectedId_ = p->getId();
                return;
            }
        }
        else if (Circle* c = dynamic_cast<Circle*>(shapePtr.get())) {
            if (std::hypot(x - c->getCenterX(), y - c->getCenterY()) < c->getRadius() + 5) {
                selectedId_ = c->getId();
                return;
            }
        }
        else if (Line* l = dynamic_cast<Line*>(shapePtr.get())) {
            if (distanceToLine(x, y, l->getStartX(), l->getStartY(), 
                               l->getEndX(), l->getEndY()) < 5) {
                selectedId_ = l->getId();
                return;
            }
        }
    }
}

//Расстояние от точки до отрезка
double CadWidget::distanceToLine(double x, double y, double x1, double y1, double x2, double y2) {
    double dx = x2 - x1, dy = y2 - y1;
    if (dx == 0 && dy == 0) return std::hypot(x - x1, y - y1);
    
    double t = ((x - x1) * dx + (y - y1) * dy) / (dx * dx + dy * dy);
    t = std::max(0.0, std::min(1.0, t));
    
    double nx = x1 + t * dx, ny = y1 + t * dy;
    return std::hypot(x - nx, y - ny);
}

void CadWidget::addPointHere(double x, double y) {
    document_->addShape(std::make_unique<Point>(nextId_++, x, y));
}

void CadWidget::addLineHere(double x1, double y1, double x2, double y2) {
    if (std::hypot(x2 - x1, y2 - y1) < 5) return;
    document_->addShape(std::make_unique<Line>(nextId_++, x1, y1, x2, y2));
}

void CadWidget::addCircleHere(double cx, double cy, double radius) {
    if (radius < 5) return;
    document_->addShape(std::make_unique<Circle>(nextId_++, cx, cy, radius));
}

void CadWidget::saveToFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить чертёж", "data/", "JSON (*.json)");
    if (fileName.isEmpty()) return;
    
    if (document_->saveToFile(fileName.toStdString())) {
        QMessageBox::information(this, "Успех", "Чертёж сохранён!");
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл");
    }
}

void CadWidget::loadFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Загрузить чертёж", "data/", "JSON (*.json)");
    if (fileName.isEmpty()) return;
    
    if (document_->loadFromFile(fileName.toStdString())) {
        selectedId_ = -1;
        update();
        QMessageBox::information(this, "Успех", "Чертёж загружен!");
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить файл");
    }
}

void CadWidget::newDocument() {
    document_->clear();
    selectedId_ = -1;
    nextId_ = 100;
    update();
}