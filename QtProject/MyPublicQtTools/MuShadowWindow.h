/**
 * 自定义无边框窗体、对话框和提示框
 *
 * MuShadowWindow.h
 * 所有自定义窗口的基类，定义窗口各个位置的布局
 *
 *
 */

#ifndef MUSHADOWWINDOW_H
#define MUSHADOWWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QDialog>
#include "MuShadowWidget.h"
#include "MyTitleBar.h"
class QPainter;
class QLineEdit;
class QLabel;

/*************************************
 * ----------------------------------
 * |shadowWindow                    |
 * |  ----------------------------  |
 * |  |ShadowWidget              |  |
 * |  |  ----------------------  |  |
 * |  |  |ShadowClientWidget  |  |  |
 * |  |  | ------------------ |  |  |
 * |  |  | |TitleBar        | |  |  |
 * |  |  | ------------------ |  |  |
 * |  |  | ------------------ |  |  |
 * |  |  | |ClientWidget    | |  |  |
 * |  |  | |                | |  |  |
 * |  |  | ------------------ |  |  |
 * |  |  ----------------------  |  |
 * |  |                          |  |
 * |  ----------------------------  |
 * |                                |
 * ----------------------------------
 *************************************/
template <class Base>
class MuShadowWindow : public Base
{
public:
    explicit MuShadowWindow(bool canResize, int shadowSize, QWidget *parent = nullptr)
        : Base(parent)
        , m_shadowSize(shadowSize)
        , m_pShadowWidget(nullptr)
        , m_pClientWidget(nullptr)
    {

        pT = this;
        pT->setAttribute(Qt::WA_TranslucentBackground, true);
        pT->setWindowFlags(Qt::FramelessWindowHint);
        pT->setContentsMargins(0, 0, 0, 0);
        this->canResize = canResize;

        testwindow();
        //Base *pT = this;
        //pT->setAttribute(Qt::WA_TranslucentBackground, true);
        //pT->setWindowFlags(Qt::FramelessWindowHint);
        //pT->setContentsMargins(0, 0, 0, 0);

        //QVBoxLayout *pWindowLayout = new QVBoxLayout(pT);
        //pWindowLayout->setContentsMargins(0, 0, 0, 0);
        //pWindowLayout->setSpacing(0);

        //// 边框阴影widget
        //m_pShadowWidget = new MuShadowWidget(m_shadowSize, canResize, this);
        //m_pShadowWidget->setContentsMargins(m_shadowSize, m_shadowSize, m_shadowSize, m_shadowSize);
        //m_pShadowWidget->setAutoFillBackground(true);
        //pWindowLayout->addWidget(m_pShadowWidget);

        //QVBoxLayout *pRootLayout = new QVBoxLayout(m_pShadowWidget);
        //pRootLayout->setContentsMargins(0, 0, 0, 0);
        //pRootLayout->setSpacing(0);

        //// 窗口主要区域
        //QWidget *pShadowClientWidget = new QWidget(m_pShadowWidget);
        //pRootLayout->addWidget(pShadowClientWidget);
        //pShadowClientWidget->setAutoFillBackground(true);
        //m_pShadowClientLayout = new QVBoxLayout(pShadowClientWidget);
        //m_pShadowClientLayout->setContentsMargins(0, 0, 0, 0);
        //m_pShadowClientLayout->setSpacing(0);



        //m_pClientWidget = new QWidget(pShadowClientWidget);
        //m_pShadowClientLayout->addWidget(m_pClientWidget);

        ////m_pClientLayout = new QVBoxLayout;
        ////m_pClientWidget->setLayout(m_pClientLayout);
    }


    void testwindow()
    {
        pWindowLayout = new QVBoxLayout(pT);
        pWindowLayout->setContentsMargins(0, 0, 0, 0);
        pWindowLayout->setSpacing(0);

        // 边框阴影widget
        m_pShadowWidget = new MuShadowWidget(m_shadowSize, canResize, this);
        m_pShadowWidget->setContentsMargins(m_shadowSize, m_shadowSize, m_shadowSize, m_shadowSize);
        m_pShadowWidget->setAutoFillBackground(true);
        pWindowLayout->addWidget(m_pShadowWidget);

        pRootLayout = new QVBoxLayout(m_pShadowWidget);
        pRootLayout->setContentsMargins(0, 0, 0, 0);
        pRootLayout->setSpacing(0);

        // 窗口主要区域
        pShadowClientWidget = new QWidget(m_pShadowWidget);
        pRootLayout->addWidget(pShadowClientWidget);
        pShadowClientWidget->setAutoFillBackground(true);
        m_pShadowClientLayout = new QVBoxLayout(pShadowClientWidget);
        m_pShadowClientLayout->setContentsMargins(0, 0, 0, 0);
        m_pShadowClientLayout->setSpacing(0);



        m_pClientWidget = new QWidget(pShadowClientWidget);
        m_pShadowClientLayout->addWidget(m_pClientWidget);

        //m_pClientLayout = new QVBoxLayout;
        //m_pClientWidget->setLayout(m_pClientLayout);
    }

    void delewindow()
    {
        //if (m_titleBar->m_pButtonRestore->isVisible())
        if (0)
        {
            m_shadowSize = 0;
            m_pShadowWidget->setContentsMargins(0, 0, 0, 0);
            //m_titleBar->setWindowBorderWidth(this->m_shadowSize, this->m_shadowSize);

            //m_titleBar->test();

        }



        //delete pShadowClientWidget;
        //pShadowClientWidget = nullptr;

        //delete pRootLayout;
        //pRootLayout = nullptr;

        //delete m_pClientWidget;
        //m_pClientWidget = nullptr;

        //delete m_pShadowClientLayout;
        //m_pShadowClientLayout = nullptr;
    }



public:
    //QWidget* rootWidget() const { return m_pShadowWidget; }
    //QWidget *clientWidget() const { return m_pClientWidget; }
    //QLayout* clientLayout() const { return m_pClientLayout; }

    Base* pT;
    

    QVBoxLayout* pWindowLayout;
    QVBoxLayout* pRootLayout;
    QWidget* pShadowClientWidget;
    bool canResize;

    /**
     * @brief setClientWidget
     *  设置client替换掉旧的m_pClientWidget
     *  \warning
     *  如果调用了该函数就不能调用clientLayout()函数了，因为m_pClientLayout指针已被释放
     * @param client
     */
    void setClientWidget(QWidget *client) {
        if (client == nullptr)
            return;

        m_pShadowClientLayout->removeWidget(m_pClientWidget);
        m_pClientWidget->deleteLater();
        //m_pClientLayout->deleteLater();
        //m_pClientLayout = nullptr;
        m_pClientWidget = client;
        m_pShadowClientLayout->addWidget(m_pClientWidget);
    }

    int m_shadowSize;

    void setfather(QWidget* father)
    {
        mparent = father;
        //initTitleBar();
    }


protected:
    //MyTitleBar* m_titleBar;


private:
    //void initTitleBar()									//初始化标题
    //{
    //    {
    //        //this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    //        //this->setWindowFlags(Qt::FramelessWindowHint);

    //        // 设置窗口背景透明;
    //        //this->setAttribute(Qt::WA_TranslucentBackground);


    //        m_titleBar = new MyTitleBar(mparent);

    //        m_titleBar->setTitleIcon("Image/QAXico.png");
    //        //m_titleBar->setBackgroundColor(0, 0, 0, 1);
    //        m_titleBar->setTitleContent(QStringLiteral(" 这是标题"), 20);
    //        m_titleBar->setButtonType(MIN_MAX_BUTTON);
    //        m_titleBar->setTitleRoll(5);
    //        m_titleBar->setWindowBorderWidth(this->m_shadowSize,this->m_shadowSize);
    //        //m_titleBar->setTitleHeight(80);
    //        m_titleBar->setRightClickMenu();

    //    }
    //}


private:
    MuShadowWidget *m_pShadowWidget;
    QWidget *m_pClientWidget;
    QVBoxLayout *m_pClientLayout;
    QVBoxLayout *m_pShadowClientLayout;
    QWidget* mparent;
};

typedef MuShadowWindow<QWidget> MuCustomWindowWidget;
typedef MuShadowWindow<QDialog> MuCustomDialogWidget;
//typedef MuShadowWindow<QMessageBox> MuCustomMessageBoxWidget;

#endif // MUSHADOWWINDOW_H