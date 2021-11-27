/**
 * 自定义无边框窗体、对话框和提示框
 *
 * MainWindow.h
 * 测试用例主要窗口
 *
 *
 */

#include <QLabel>
#include <QDebug>
#include "MainWindow.h"
#include "ui_MainWindow.h"
 


MainWindow::MainWindow(QWidget *parent)
    : MuCustomWindow(parent)
    , ui(new Ui::MainWindow)

{
    setWindowTitle("Test Custom Window");
    resize(800, 600);

    QWidget *pClientWidget = new QWidget(this);
    ui->setupUi(pClientWidget);

    

    // 设置中心客户区域
    //setClientWidget(pClientWidget);

    

}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    qDebug() << "触发";
    Q_UNUSED(eventType);
    MSG* msg = static_cast<MSG*>(message);
    switch (msg->message)
    {
    case WM_DISPLAYCHANGE:
        qDebug() << "改变了大小";
        return 1;
    }
    return false;
}