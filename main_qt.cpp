#include <QApplication>

#include "gui/main_window.h"
#include "document.h"
#include "point.h"
#include "line.h"
#include "circle.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    Document doc;
    
    // Тестовые фигуры
    doc.addShape(std::make_unique<Point>(1, 100, 100));
    doc.addShape(std::make_unique<Line>(2, 200, 200, 400, 300));
    doc.addShape(std::make_unique<Circle>(3, 500, 150, 80));
    
    MainWindow window(&doc);
    window.show();
    
    return app.exec();
}