#include <gtest/gtest.h>

#include <memory>
#include <sstream>
#include "point.h"
#include "line.h"
#include "circle.h"
#include "document.h"

/*
    Тесты для класса Point:
        Конструктор,
        Наследуемые функции
*/

TEST(PointTest, ConstructorAndGetters) {
    Point p(1, 2.5, -1.3);
    
    EXPECT_EQ(p.getId(), 1);
    EXPECT_DOUBLE_EQ(p.getX(), 2.5);
    EXPECT_DOUBLE_EQ(p.getY(), -1.3);
}

TEST(PointTest, Move) {
    Point p(1, 2.5, -1.3);
    
    p.move(10, -5);
    
    EXPECT_DOUBLE_EQ(p.getX(), 12.5);
    EXPECT_DOUBLE_EQ(p.getY(), -6.3);
}

TEST(PointTest, Scale) {
    Point p(1, 2.5, -1.3);
    
    p.scale(2.0);
    
    EXPECT_DOUBLE_EQ(p.getX(), 5.0);
    EXPECT_DOUBLE_EQ(p.getY(), -2.6);
}

TEST(PointTest, GetInfo) {
    Point p(1, 2.5, -1.3);
    
    std::string info = p.getInfo();
    
    EXPECT_TRUE(info.find("Point") != std::string::npos);
    EXPECT_TRUE(info.find("2.5") != std::string::npos);
    EXPECT_TRUE(info.find("-1.3") != std::string::npos);
}

/*
    Тесты для класса Line:
        Конструктор,
        Наследуемые функции
*/
TEST(LineTest, ConstructorAndGetters) {
    Line l(2, 0, 0, 5, 5);
    
    EXPECT_EQ(l.getId(), 2);
    EXPECT_DOUBLE_EQ(l.getStartX(), 0.0);
    EXPECT_DOUBLE_EQ(l.getStartY(), 0.0);
    EXPECT_DOUBLE_EQ(l.getEndX(), 5.0);
    EXPECT_DOUBLE_EQ(l.getEndY(), 5.0);
}

TEST(LineTest, Move) {
    Line l(2, 0, 0, 5, 5);
    
    l.move(10, -5);
    
    EXPECT_DOUBLE_EQ(l.getStartX(), 10.0);
    EXPECT_DOUBLE_EQ(l.getStartY(), -5.0);
    EXPECT_DOUBLE_EQ(l.getEndX(), 15.0);
    EXPECT_DOUBLE_EQ(l.getEndY(), 0.0);
}

TEST(LineTest, Scale) {
    Line l(2, 0, 0, 5, 5);
    
    l.scale(2.0);
    
    EXPECT_DOUBLE_EQ(l.getStartX(), 0.0);
    EXPECT_DOUBLE_EQ(l.getStartY(), 0.0);
    EXPECT_DOUBLE_EQ(l.getEndX(), 10.0);
    EXPECT_DOUBLE_EQ(l.getEndY(), 10.0);
}

/*
    Тесты для класса Circle:
        Конструктор,
        Наследуемые функции,
        Собственные функции
*/

TEST(CircleTest, ConstructorAndGetters) {
    Circle c(3, 10, 10, 4.5);
    
    EXPECT_EQ(c.getId(), 3);
    EXPECT_DOUBLE_EQ(c.getCenterX(), 10.0);
    EXPECT_DOUBLE_EQ(c.getCenterY(), 10.0);
    EXPECT_DOUBLE_EQ(c.getRadius(), 4.5);
}

TEST(CircleTest, Move) {
    Circle c(3, 10, 10, 4.5);
    
    c.move(-5, -5);
    
    EXPECT_DOUBLE_EQ(c.getCenterX(), 5.0);
    EXPECT_DOUBLE_EQ(c.getCenterY(), 5.0);
    EXPECT_DOUBLE_EQ(c.getRadius(), 4.5);
}

TEST(CircleTest, Scale) {
    Circle c(3, 10, 10, 4.5);
    
    c.scale(2.0);
    
    EXPECT_DOUBLE_EQ(c.getCenterX(), 20.0);
    EXPECT_DOUBLE_EQ(c.getCenterY(), 20.0);
    EXPECT_DOUBLE_EQ(c.getRadius(), 9.0);
}

TEST(CircleTest, NegativeRadiusProtection) {
    Circle c(3, 0, 0, -5.0);
    
    EXPECT_DOUBLE_EQ(c.getRadius(), 0.001);
}

/*
    Тесты для класса Document:
        Конструктор,
        CRUD,
        null проверки
*/

TEST(DocumentTest, AddAndSize) {
    Document doc;
    
    doc.addShape(std::make_unique<Point>(1, 0, 0));
    doc.addShape(std::make_unique<Line>(2, 0, 0, 5, 5));
    
    EXPECT_EQ(doc.size(), 2);
}

TEST(DocumentTest, FindById) {
    Document doc;
    doc.addShape(std::make_unique<Point>(1, 0, 0));
    doc.addShape(std::make_unique<Line>(2, 0, 0, 5, 5));
    doc.addShape(std::make_unique<Circle>(3, 10, 10, 4.5));
    
    Shape* found = doc.findById(2);
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->getId(), 2);
    
    Shape* notFound = doc.findById(999);
    EXPECT_EQ(notFound, nullptr);
}

TEST(DocumentTest, RemoveById) {
    Document doc;
    doc.addShape(std::make_unique<Point>(1, 0, 0));
    doc.addShape(std::make_unique<Line>(2, 0, 0, 5, 5));
    
    bool removed = doc.removeById(1);
    EXPECT_TRUE(removed);
    EXPECT_EQ(doc.size(), 1);
    
    bool removedAgain = doc.removeById(1);
    EXPECT_FALSE(removedAgain);
}

TEST(DocumentTest, MoveShape) {
    Document doc;
    doc.addShape(std::make_unique<Point>(1, 2.5, -1.3));
    
    bool moved = doc.moveShape(1, 10, -5);
    EXPECT_TRUE(moved);
    
    Shape* shape = doc.findById(1);
    Point* p = dynamic_cast<Point*>(shape);
    ASSERT_NE(p, nullptr);
    EXPECT_DOUBLE_EQ(p->getX(), 12.5);
    EXPECT_DOUBLE_EQ(p->getY(), -6.3);
}

TEST(DocumentTest, ScaleShape) {
    Document doc;
    doc.addShape(std::make_unique<Circle>(1, 10, 10, 4.5));
    
    bool scaled = doc.scaleShape(1, 2.0);
    EXPECT_TRUE(scaled);
    
    Shape* shape = doc.findById(1);
    Circle* c = dynamic_cast<Circle*>(shape);
    ASSERT_NE(c, nullptr);
    EXPECT_DOUBLE_EQ(c->getRadius(), 9.0);
}

//Нашел ошибку - вызов shape->getInfo() до проверки if(shape)
TEST(DocumentTest, MoveNonexistentShape) {
    Document doc;
    doc.addShape(std::make_unique<Point>(1, 0, 0));
    
    bool moved = doc.moveShape(999, 10, 10);
    EXPECT_FALSE(moved);
}

//Нашел ошибку - вызов shape->getInfo() до проверки if(shape)
TEST(DocumentTest, ScaleNonexistentShape) {
    Document doc;
    doc.addShape(std::make_unique<Point>(1, 0, 0));
    
    bool scaled = doc.scaleShape(999, 2.0);
    EXPECT_FALSE(scaled);
}

/*
    Запуск тестов
*/

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}