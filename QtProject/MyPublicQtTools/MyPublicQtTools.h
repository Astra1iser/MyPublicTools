#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MyPublicQtTools.h"
#include "stdafx.h"

class MyPublicQtTools : public QMainWindow
{
    Q_OBJECT

public:
    MyPublicQtTools(QWidget *parent = Q_NULLPTR);
    ~MyPublicQtTools()
    {
        //delete ui;
    }





private:
    Ui::MyPublicQtToolsClass ui;
};
