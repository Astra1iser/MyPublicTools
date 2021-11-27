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



CString GetHostNameFromURL(const CString& strURL)
{
    static const TCHAR pszBeginPattern[] = _T("://");
    static const TCHAR pszEndPattern[] = _T("/");

    int iStart = strURL.Find(pszBeginPattern);
    if (iStart <= 0)
        return strURL;

    iStart += _countof(pszBeginPattern) - 1;

    int iEnd = strURL.Find(pszEndPattern, iStart);
    if (iEnd <= 0)
        return strURL.Mid(iStart);

    return strURL.Mid(iStart, iEnd - iStart);
}

CString ReplaceDomainNameByIP(const CString& strURL, const CString& strIP)
{
    static const TCHAR pszStartPattern[] = _T("://");
    static const TCHAR pszEndPattern[] = _T("/");

    int iStart = strURL.Find(pszStartPattern);
    if (iStart < 0)
        return strURL;

    int iEnd = strURL.Find(pszEndPattern, iStart + _countof(pszStartPattern) - 1);
    if (iEnd > 0)
    {
        return strURL.Left(iStart + _countof(pszStartPattern) - 1) + strIP + strURL.Mid(iEnd);
    }

    return strURL.Left(iStart + _countof(pszStartPattern) - 1) + strIP;
}



MyPublicQtTools::MyPublicQtTools(QWidget *parent)
    :QMainWindow(parent)
{
   // QWidget* pClientWidget = new QWidget(this);
    //ui.setupUi(pClientWidget);


    ui.setupUi(this);

    initTitleBar();

    // 设置中心客户区域
    //setClientWidget(pClientWidget);
    resize(800, 600);

    //setfather(this);

    int errorc = 0;
    //initTitleBar();
    //initShadow();
    //bool ddd = zhengze(L"\\R1egistry\\Machine\\System\\ControlSet???\\Hardware Profiles\\????");
    //bool ddd = bijiao(L"\\Registry\\Machine\\System\\ControlSet???");

    //cout << GetSysErrorMessage(&errorc, &szMsg);


    //qstring转cstring
    //string str = m_QStrFile.toLocal8Bit().data();
    //CString CStrbuffer = str.c_str();

    //cstring转qstring





    /*
    此行代码可以带回Aero效果，同时也带回了标题栏和边框,在nativeEvent()会再次去掉标题栏
    主要的关键是WS_THICKFRAME，可以实现窗口停靠边缘自动改变大小功能和Aero效果
    */
    //this line will get titlebar/thick frame/Aero back, which is exactly what we want
    //we will get rid of titlebar and thick frame again in nativeEvent() later
    HWND hwnd = (HWND)this->winId();
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_THICKFRAME);

    //保留一个像素的边框宽度，否则系统不会绘制边框阴影
    //
    //we better left 1 piexl width of border untouch, so OS can draw nice shadow around it
    const MARGINS shadow = { 1, 1, 1, 1 };
    DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);













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
    //m_titleBar->setBackgroundColor(0, 0, 0, 1);
    m_titleBar->setTitleContent(QStringLiteral(" 这是标题这是标题这是标题这是标题这是标题这是标题这是标题这是标题这是标题这是标题"),20);
    m_titleBar->setButtonType(MIN_MAX_BUTTON);
    m_titleBar->setTitleRoll(5);
    //m_titleBar->setWindowBorderWidth(20);
    //m_titleBar->setTitleHeight(80);
    m_titleBar->setRightClickMenu();


    //m_titleBar->setTitleWidth(1);



    //connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    //connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
    //connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
    //connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
} 


 //void MyPublicQtTools::initShadow()
 //{
 //    m_shadow = new MuShadowWindow<QMainWindow>(1,5,this);



 //}


 bool MyPublicQtTools::nativeEvent(const QByteArray& eventType, void* message, long* result)
 {
     MSG* param = static_cast<MSG*>(message);
     static long cnt = 10000;
     //printf("######:%x --- %d\n", param->message, ++cnt);
     switch (param->message)
     {
     case WM_NCCALCSIZE: {
         /*
         此消息用于处理非客户区域，比如边框的绘制
         返回false,就是按系统的默认处理，如果返回true,而不做任何绘制，则非客户区域
         就不会被绘制，就相当于没有绘制非客户区域，所以就会看不到非客户区域的效果
         */
         return true;
     }
     case WM_NCHITTEST:
     {
         const LONG border_width = 3;
         RECT winrect;
         GetWindowRect(HWND(winId()), &winrect);

         long x = GET_X_LPARAM(param->lParam);
         long y = GET_Y_LPARAM(param->lParam);

         /*
         只用这种办法设置动态改变窗口大小比手动通过鼠标事件效果好，可以
         避免闪烁问题
         */
         //left border
         if (x >= winrect.left && x < winrect.left + border_width)
         {
             *result = HTLEFT;
             return true;
         }
         //right border
         if (x < winrect.right && x >= winrect.right - border_width)
         {
             *result = HTRIGHT;
             return true;
         }

         //bottom border
         if (y < winrect.bottom && y >= winrect.bottom - border_width)
         {
             *result = HTBOTTOM;
             return true;
         }
         //top border
         if (y >= winrect.top && y < winrect.top + border_width)
         {
             *result = HTTOP;
             return true;
         }
         //bottom left corner
         if (x >= winrect.left && x < winrect.left + border_width &&
             y < winrect.bottom && y >= winrect.bottom - border_width)
         {
             *result = HTBOTTOMLEFT;
             return true;
         }
         //bottom right corner
         if (x < winrect.right && x >= winrect.right - border_width &&
             y < winrect.bottom && y >= winrect.bottom - border_width)
         {
             *result = HTBOTTOMRIGHT;
             return true;
         }
         //top left corner
         if (x >= winrect.left && x < winrect.left + border_width &&
             y >= winrect.top && y < winrect.top + border_width)
         {
             *result = HTTOPLEFT;
             return true;
         }
         //top right corner
         if (x < winrect.right && x >= winrect.right - border_width &&
             y >= winrect.top && y < winrect.top + border_width)
         {
             *result = HTTOPRIGHT;
             return true;
         }


         return QWidget::nativeEvent(eventType, message, result);
     }

     case WM_WININICHANGE:
     {
         if (m_titleBar->m_pButtonRestore->isVisible())
         {
             qDebug() << YString("触发");
             m_titleBar->test();
             return true;
         }
     }
     }

     return QWidget::nativeEvent(eventType, message, result);
 }






 //void MyPublicQtTools::paintEvent(QPaintEvent* event)
 //{
 //    QPainterPath path;
 //    path.setFillRule(Qt::WindingFill);
 //    QRectF rect(10, 10, this->width() - 20, this->height() - 20);
 //    path.addRoundRect(rect, 8, 8);

 //    QPainter painter(this);
 //    painter.setRenderHint(QPainter::Antialiasing, true);
 //    painter.fillPath(path, QBrush(Qt::white));

 //    QColor color(0, 0, 0, 50);
 //    for (int i = 0; i < 10; i++) {
 //        QPainterPath path;
 //        path.setFillRule(Qt::WindingFill);
 //        path.addRect(10 - i, 10 - i, this->width() - (10 - i) * 2, this->height() - (10 - i) * 2);
 //        color.setAlpha(150 - qSqrt(i) * 50);
 //        painter.setPen(color);
 //        painter.drawPath(path);
 //    }
 //    //setWindowState(Qt::WindowMinimized);
 //    //setWindowState(Qt::WindowMaximized);

 //}