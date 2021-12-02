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


void fun2(int b);

void fun1(int b)
{
    int a = b;
    fun2(a);
}

void fun2(int b)
{
    int a = b;
    fun1(a);
}



MyPublicQtTools::MyPublicQtTools(QWidget *parent)
    :QWidget(parent)
{

    ui.setupUi(this);

    initTitleBar();






    resize(600, 200);




    int errorc = 0;
    setWindowIcon(QIcon("Helmet.ico"));//设置状态栏图标



//    QTimer* timer = new QTimer(this);
//    connect
//    (
//        ui.loopbutton, &QPushButton::clicked, this, [=]()
//        {
//            if (timer->isActive())
//            {
//                timer->stop();
//                ui.loopbutton->setText(YString("开启时间"));
//                return;
//            }
//            //ui.curtime->setText(CS2QS(L"%s,%d",WhoIsUser(GetCurrentProcessId()),222));
//            ui.loopbutton->setText(YString("关闭时间"));
//            timer->start(1000);
//        }
//    );
//
//    connect
//    (
//        timer, &QTimer::timeout, this, [=]()
//        {
//            ui.curtime->setText(QTime::currentTime().toString());
//            ui.curtime->setToolTip(ToolTipWrap(QTime::currentTime().toString(), 100, fontMetrics()));
//        }
//    );
//

    ui.C_collapse_Button->setText(YString("C++崩溃"));
    ui.Win_collapse_Button->setText(YString("Windows崩溃"));


    connect
    (ui.C_collapse_Button, &QPushButton::clicked, this, [=]()
        {
            HANDLE myHandle;
            StartPrograme(L"MyPublicTools.exe", myHandle,L"",1,0);
        }
    );

    connect
    (ui.Win_collapse_Button, &QPushButton::clicked, this, [=]()
        {
            //HANDLE myHandle;
            //StartPrograme(L"MyPublicTools2.exe", myHandle, L"", 1, 0);
            fun1(2);
        }
    );




}


 void MyPublicQtTools::initTitleBar()
{
   
    m_titleBar = new MyTitleBar(this);
    m_titleBar->setTitleIcon("Image/QAXico.png");
    //m_titleBar->setBackgroundColor(0, 0, 0, 1);
    m_titleBar->setTitleContent(QStringLiteral("天擎客户端开发二组---崩溃触发工具"),15);
    m_titleBar->setButtonType(MIN_MAX_BUTTON);
    //m_titleBar->setButtonType(ONLY_CLOSE_BUTTON);
    m_titleBar->setTitleRoll(20);
    //m_titleBar->setWindowBorderWidth(20);
    //m_titleBar->setTitleHeight(80);
    m_titleBar->setRightClickMenu();
    m_titleBar->setStretch(FALSE);

} 


 void MyPublicQtTools::paintEvent(QPaintEvent* event)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter painter(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
 }


 bool MyPublicQtTools::nativeEvent(const QByteArray& eventType, void* message, long* result)
 {
     /*
     事件处理:
     1、返迥true : 告诉Qt已经处理了这个事件,不要做其他处理
     2、返迥false : Qt会把这个事件传递给它的父窗口部件来处理
     3、返迥基类的event(QEvent*) : Qt把这个事件交给它的基类来处理(注意与返回false的区别)
     事件过滤器 :
     1、返迥true : 告诉Qt已经处理了这个事件,不要做其他处理
     2、放回false : 该函数未处理，交给Qt, Qt将 会把这个事件发送给指定的目标对象
     3、迥基类的eventFileter(QObject*, QEvent*) : Qt把这个事件交给它的基类来处理(有些窗口部件会对它们的子窗口部件进行监控)
     (注意与返回false的区别)。
     */

     Q_UNUSED(eventType);
     MSG* msg = static_cast<MSG*>(message);


     //QWidget* widget = QWidget::find(reinterpret_cast<WId>(msg->hwnd));
     //if (!widget)
     //    return false;


     switch (msg->message)
     {
     case WM_NCCALCSIZE:
     {
         /*
         此消息用于处理非客户区域，比如边框的绘制
         返回false,就是按系统的默认处理，如果返回true,而不做任何绘制，则非客户区域
         就不会被绘制，就相当于没有绘制非客户区域，所以就会看不到非客户区域的效果
         */       
         return true;
     }
     case WM_NCHITTEST:
     {
         if (m_titleBar->m_isStretch)
         {
             const LONG border_width = 3;
             RECT winrect;
             GetWindowRect(HWND(winId()), &winrect);

             long x = GET_X_LPARAM(msg->lParam);
             long y = GET_Y_LPARAM(msg->lParam);

             /*
             只用这种办法设置动态改变窗口大小比手动通过鼠标事件效果好，可以
             避免闪烁问题
             */
             //左边界
             if (x >= winrect.left && x < winrect.left + border_width)
             {
                 *result = HTLEFT;
                 return true;
             }
             //右边界
             if (x < winrect.right && x >= winrect.right - border_width)
             {
                 *result = HTRIGHT;
                 return true;
             }

             //底部边界
             if (y < winrect.bottom && y >= winrect.bottom - border_width)
             {
                 *result = HTBOTTOM;
                 return true;
             }
             //顶部边界
             if (y >= winrect.top && y < winrect.top + border_width)
             {
                 *result = HTTOP;
                 return true;
             }
             //左下角
             if (x >= winrect.left && x < winrect.left + border_width &&
                 y < winrect.bottom && y >= winrect.bottom - border_width)
             {
                 *result = HTBOTTOMLEFT;
                 return true;
             }
             //右下角
             if (x < winrect.right && x >= winrect.right - border_width &&
                 y < winrect.bottom && y >= winrect.bottom - border_width)
             {
                 *result = HTBOTTOMRIGHT;
                 return true;
             }
             //左上角
             if (x >= winrect.left && x < winrect.left + border_width &&
                 y >= winrect.top && y < winrect.top + border_width)
             {
                 *result = HTTOPLEFT;
                 return true;
             }
             //右上角
             if (x < winrect.right && x >= winrect.right - border_width &&
                 y >= winrect.top && y < winrect.top + border_width)
             {
                 *result = HTTOPRIGHT;
                 return true;
             }
         }
         return QWidget::nativeEvent(eventType, message, result);
     }




     //case WM_GETMINMAXINFO:
     //{
     //    if (::IsZoomed(msg->hwnd)) {
     //        //isMaximized_ = true;
     //        RECT frame = { 0, 0, 0, 0 };
     //        AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);
     //        frame.left = abs(frame.left);
     //        frame.top = abs(frame.bottom);
     //        widget->setContentsMargins(frame.left, frame.top, frame.right, frame.bottom);
     //    }
     //    else {
     //        widget->setContentsMargins(0, 0, 0, 0);
     //        //isMaximized_ = false;
     //    }

     //    *result = ::DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
     //    return true;
     //}




    //客户区发生变化时windows发送过来的消息
     //case WM_WININICHANGE:
     //{
     //    if (m_titleBar->m_isMaximize)
     //    {
     //        m_titleBar->m_PreviousFactRect = m_titleBar->getDesktopRect();
     //        m_titleBar->m_CorrectionType = TRUE;
     //        //我们只是触发一个消息,并不想重写此消息处理,所以返回false,让QT后续去处理
     //        //return false;
     //        return QWidget::nativeEvent(eventType, message, result);
     //    }
     //}
     }

     return QWidget::nativeEvent(eventType, message, result);
 }