#pragma once

#include <QMainWindow>
#include <memory>
#include "Document.h"

class CadWidget;

class MainWindow : public QMainWindow {

public:
    MainWindow(Document* doc, QWidget* parent = nullptr);

private slots:
    void onNew();
    void onSave();
    void onLoad();
    void onToolChanged(int index);

private:
    Document* document_;
    CadWidget* cadWidget_;
    void createMenuBar();
};