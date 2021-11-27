/**
 * 自定义无边框窗体、对话框和提示框
 *
 * MuCustomWindow.cpp
 * 自定义无边框窗体、对话框和提示框的实现文件，主要API实现文件
 *
 *
 */

#include <QDialogButtonBox>
#include <QLabel>
#include <QCheckBox>
#include <QLabel>
#include "MuCustomWindow.h"


#ifdef Q_CC_MSVC
#pragma comment(lib, "user32.lib")
#endif

MuCustomWindow::MuCustomWindow(QWidget *parent)
    : MuShadowWindow<QWidget>(0, 10, parent)
{
    //if (parent != nullptr) {
    //    setWindowTitle(parent->windowTitle());
    //    setWindowIcon(parent->windowIcon());
    //} else {
    //    this->setWindowTitle("Custom Window");
    //    this->setWindowIcon(QIcon(":/images/logo.jpg"));
    //}
    //resize(800, 1600);
}

bool MuCustomWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    Q_UNUSED(eventType);
    MSG* msg = static_cast<MSG*>(message);


    switch (msg->message)
    {
    case WM_WININICHANGE:
        delewindow();
        return 1;
    }
    return false;
}



/*
 * -----------------------------
 * |MuWinAeroShadowWindow      |
 * | ------------------------- |
 * | |m_pContainerWidget     | |
 * | | --------------------- | |
 * | | |titleBar           | | |
 * | | --------------------- | |
 * | | --------------------- | |
 * | | |m_pClientWidget    | | |
 * | | |                   | | |
 * | | |                   | | |
 * | | --------------------- | |
 * | ------------------------- |
 * -----------------------------
 */



