/**
 * 自定义无边框窗体、对话框和提示框
 *
 * MainWindow.h
 * 测试用例主要窗口
 *
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "MuCustomWindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public MuCustomWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:


private:


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
