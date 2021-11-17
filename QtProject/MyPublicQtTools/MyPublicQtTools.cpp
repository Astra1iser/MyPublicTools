#include "MyPublicQtTools.h"




MyPublicQtTools::MyPublicQtTools(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    int errorc = 0;

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
            ui.curtime->setText(CS2QS(L"%s,%d",WhoIsUser(GetCurrentProcessId()),222));
            //timer->start(1000);
        }
    );

    connect
    (
        timer, &QTimer::timeout, this, [=]()
        {
            ui.curtime->setText(QTime::currentTime().toString());
        }
    );






}
