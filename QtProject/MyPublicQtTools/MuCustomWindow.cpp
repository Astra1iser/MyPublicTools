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
    : MuShadowWindow<QWidget>(true, 20, parent)
{
    //if (parent != nullptr) {
    //    setWindowTitle(parent->windowTitle());
    //    setWindowIcon(parent->windowIcon());
    //} else {
    //    this->setWindowTitle("Custom Window");
    //    this->setWindowIcon(QIcon(":/images/logo.jpg"));
    //}
    resize(800, 600);
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




void MuWinAeroShadowWindow::setClientWidget(QWidget *client)
{
    if (client == nullptr)
        return;

    m_pContainerLayout->removeWidget(m_pClientWidget);
    m_pClientWidget->deleteLater();
    m_pClientLayout->deleteLater();
    m_pClientLayout = nullptr;
    m_pClientWidget = client;
    m_pContainerLayout->addWidget(m_pClientWidget);
}
