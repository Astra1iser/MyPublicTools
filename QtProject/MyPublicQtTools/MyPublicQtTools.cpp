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
    setWindowIcon(QIcon("Helmet.ico"));//����״̬��ͼ��



//    QTimer* timer = new QTimer(this);
//    connect
//    (
//        ui.loopbutton, &QPushButton::clicked, this, [=]()
//        {
//            if (timer->isActive())
//            {
//                timer->stop();
//                ui.loopbutton->setText(YString("����ʱ��"));
//                return;
//            }
//            //ui.curtime->setText(CS2QS(L"%s,%d",WhoIsUser(GetCurrentProcessId()),222));
//            ui.loopbutton->setText(YString("�ر�ʱ��"));
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

    ui.C_collapse_Button->setText(YString("C++����"));
    ui.Win_collapse_Button->setText(YString("Windows����"));


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
    m_titleBar->setTitleContent(QStringLiteral("����ͻ��˿�������---������������"),15);
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
     �¼�����:
     1������true : ����Qt�Ѿ�����������¼�,��Ҫ����������
     2������false : Qt�������¼����ݸ����ĸ����ڲ���������
     3�����Ļ����event(QEvent*) : Qt������¼��������Ļ���������(ע���뷵��false������)
     �¼������� :
     1������true : ����Qt�Ѿ�����������¼�,��Ҫ����������
     2���Ż�false : �ú���δ��������Qt, Qt�� �������¼����͸�ָ����Ŀ�����
     3���Ļ����eventFileter(QObject*, QEvent*) : Qt������¼��������Ļ���������(��Щ���ڲ���������ǵ��Ӵ��ڲ������м��)
     (ע���뷵��false������)��
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
         ����Ϣ���ڴ���ǿͻ����򣬱���߿�Ļ���
         ����false,���ǰ�ϵͳ��Ĭ�ϴ����������true,�������κλ��ƣ���ǿͻ�����
         �Ͳ��ᱻ���ƣ����൱��û�л��Ʒǿͻ��������Ծͻῴ�����ǿͻ������Ч��
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
             ֻ�����ְ취���ö�̬�ı䴰�ڴ�С���ֶ�ͨ������¼�Ч���ã�����
             ������˸����
             */
             //��߽�
             if (x >= winrect.left && x < winrect.left + border_width)
             {
                 *result = HTLEFT;
                 return true;
             }
             //�ұ߽�
             if (x < winrect.right && x >= winrect.right - border_width)
             {
                 *result = HTRIGHT;
                 return true;
             }

             //�ײ��߽�
             if (y < winrect.bottom && y >= winrect.bottom - border_width)
             {
                 *result = HTBOTTOM;
                 return true;
             }
             //�����߽�
             if (y >= winrect.top && y < winrect.top + border_width)
             {
                 *result = HTTOP;
                 return true;
             }
             //���½�
             if (x >= winrect.left && x < winrect.left + border_width &&
                 y < winrect.bottom && y >= winrect.bottom - border_width)
             {
                 *result = HTBOTTOMLEFT;
                 return true;
             }
             //���½�
             if (x < winrect.right && x >= winrect.right - border_width &&
                 y < winrect.bottom && y >= winrect.bottom - border_width)
             {
                 *result = HTBOTTOMRIGHT;
                 return true;
             }
             //���Ͻ�
             if (x >= winrect.left && x < winrect.left + border_width &&
                 y >= winrect.top && y < winrect.top + border_width)
             {
                 *result = HTTOPLEFT;
                 return true;
             }
             //���Ͻ�
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




    //�ͻ��������仯ʱwindows���͹�������Ϣ
     //case WM_WININICHANGE:
     //{
     //    if (m_titleBar->m_isMaximize)
     //    {
     //        m_titleBar->m_PreviousFactRect = m_titleBar->getDesktopRect();
     //        m_titleBar->m_CorrectionType = TRUE;
     //        //����ֻ�Ǵ���һ����Ϣ,��������д����Ϣ����,���Է���false,��QT����ȥ����
     //        //return false;
     //        return QWidget::nativeEvent(eventType, message, result);
     //    }
     //}
     }

     return QWidget::nativeEvent(eventType, message, result);
 }