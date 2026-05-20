#include <QMenuBar>
#include <QToolBar>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QStatusBar>

#include "MainWindow.h"
#include "CadWidget.h"

MainWindow::MainWindow(Document* doc, QWidget* parent)
    : QMainWindow(parent), document_(doc) {
    
    setWindowTitle("MicroCAD");
    resize(900, 700);
    
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    
    QVBoxLayout* layout = new QVBoxLayout(central);
    
    QToolBar* toolBar = addToolBar("Инструменты");
    
    QComboBox* toolBox = new QComboBox(this);
    toolBox->addItem("Выбор (Select)", static_cast<int>(Tool::Select));
    toolBox->addItem("Точка (Point)", static_cast<int>(Tool::DrawPoint));
    toolBox->addItem("Линия (Line)", static_cast<int>(Tool::DrawLine));
    toolBox->addItem("Круг (Circle)", static_cast<int>(Tool::DrawCircle));
    toolBar->addWidget(toolBox);
    
    cadWidget_ = new CadWidget(document_, this);
    layout->addWidget(cadWidget_);
    
    statusBar()->showMessage("Готов к работе");
    
    createMenuBar();
    
    connect(toolBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onToolChanged);
}

void MainWindow::createMenuBar() {
    QMenu* fileMenu = menuBar()->addMenu("&Файл");
    
    QAction* newAction = fileMenu->addAction("&Новый");
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::onNew);
    
    QAction* loadAction = fileMenu->addAction("&Открыть...");
    loadAction->setShortcut(QKeySequence::Open);
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoad);
    
    QAction* saveAction = fileMenu->addAction("&Сохранить...");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSave);
    
    fileMenu->addSeparator();
    
    QAction* exitAction = fileMenu->addAction("&Выход");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::onNew() {
    cadWidget_->newDocument();
    statusBar()->showMessage("Новый документ создан", 2000);
}

void MainWindow::onSave() {
    cadWidget_->saveToFile();
}

void MainWindow::onLoad() {
    cadWidget_->loadFromFile();
    statusBar()->showMessage("Чертёж загружен", 2000);
}

void MainWindow::onToolChanged(int index) {
    QComboBox* box = qobject_cast<QComboBox*>(sender());
    if (box) {
        Tool tool = static_cast<Tool>(box->currentData().toInt());
        cadWidget_->setTool(tool);
        
        QString toolName = box->currentText();
        statusBar()->showMessage("Инструмент: " + toolName, 2000);
    }
}