#pragma once
#include "MyPublicQtTools.h"
//#include "..\..\Include\QTInclude\MyQHeaderView\MyQHeaderView.cpp"
//#include "..\..\Include\QTInclude\MyQTTitleBar\MyTitleBar.cpp"

//#include <MyTitleBar.cpp>


BOOL zhengze(CString abc)
{

    LPCTSTR lpszName = abc;
    wstring strName(lpszName);
    std::vector<std::string> str{ wstring2string(strName) };

    std::string pattern{ "^\\\\Registry\\\\Machine\\\\System\\\\ControlSet+[-A-Za-z0-9+&@#/%?=~_|!:,.;]+\\\\Hardware Profiles\\\\+[-A-Za-z0-9+&@#/%?=~_|!:,.;]" };
    std::regex re(pattern);

    for (auto tmp : str) {
        bool ret = std::regex_search(tmp, re);
        if (!ret)
        {
            return false;
        }
    }
    return true;
}


MyPublicQtTools::MyPublicQtTools(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    int errorc = 0;
    initTitleBar();

    //bool ddd = zhengze(L"\\R1egistry\\Machine\\System\\ControlSet???\\Hardware Profiles\\????");
    //bool ddd = bijiao(L"\\Registry\\Machine\\System\\ControlSet???");




    //cout << GetSysErrorMessage(&errorc, &szMsg);


    //qstring转cstring
    //string str = m_QStrFile.toLocal8Bit().data();
    //CString CStrbuffer = str.c_str();

    //cstring转qstring
    QTimer* timer = new QTimer(this);
    connect
    (
        ui.loopbutton, &QPushButton::clicked, this, [=]()
        {
            if (timer->isActive())
            {
                timer->stop();
                ui.loopbutton->setText(YString("开启时间"));
                return;
            }
            //ui.curtime->setText(CS2QS(L"%s,%d",WhoIsUser(GetCurrentProcessId()),222));
            ui.loopbutton->setText(YString("关闭时间"));
            timer->start(1000);
        }
    );

    connect
    (
        timer, &QTimer::timeout, this, [=]()
        {
            ui.curtime->setText(QTime::currentTime().toString());
            ui.curtime->setToolTip(ToolTipWrap(QTime::currentTime().toString(), 100, fontMetrics()));
        }
    );






}




// void MyPublicQtTools::onButtonMinClicked()
//{
//    showMinimized();
//}
//
//void MyPublicQtTools::onButtonRestoreClicked()
//{
//    QPoint windowPos;
//    QSize windowSize;
//    m_titleBar->getRestoreInfo(windowPos, windowSize);
//    this->setGeometry(QRect(windowPos, windowSize));
//}
//
//void MyPublicQtTools::onButtonMaxClicked()
//{
//    m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
//    QRect desktopRect = QApplication::desktop()->availableGeometry();
//    QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
//    setGeometry(FactRect);
//}
//
//void MyPublicQtTools::onButtonCloseClicked()
//{
//    close();
//}

 void MyPublicQtTools::initTitleBar()
{
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    //this->setWindowFlags(Qt::FramelessWindowHint);

    // 设置窗口背景透明;
    //this->setAttribute(Qt::WA_TranslucentBackground);


    m_titleBar = new MyTitleBar(this);
    
    m_titleBar->setTitleIcon("Image/QAXico.png");
    m_titleBar->setTitleContent(QStringLiteral(" 这是标题这是标题这是标题这是标题这是标题这是标题这是标题这是标题这是标题这是标题"),10);
    m_titleBar->setButtonType(MIN_MAX_BUTTON);
    m_titleBar->setTitleRoll();
    //m_titleBar->setTitleWidth(1);


    //connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    //connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
    //connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
    //connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
} 