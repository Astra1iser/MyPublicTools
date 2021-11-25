/**
 * 自定义无边框窗体、对话框和提示框
 *
 * MuCustomWindow.h
 * 自定义无边框窗体、对话框和提示框的实现文件，主要API头文件
 *
 *
 */

#ifndef MUCUSTOMWINDOW_H
#define MUCUSTOMWINDOW_H

#include <QDialogButtonBox>
#include <QHash>
#include "MuShadowWindow.h"

class  MuCustomWindow : public MuShadowWindow<QWidget>
{
public:
    explicit MuCustomWindow(QWidget *parent = nullptr);

protected:
};

#ifdef Q_OS_WIN32
/**
 * @brief The MuWinAeroShadowWindow class
 *  实现Aero部分特效的窗体
 */
class  MuWinAeroShadowWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MuWinAeroShadowWindow(QWidget *parent = nullptr);

    QWidget *clientWidget() const { return m_pClientWidget; }
    QVBoxLayout *clientLayout() const { return m_pClientLayout; }

    void setTitleBarHeight(int height);

    /**
     * @brief setClientWidget
     *  设置client替换掉旧的m_pClientWidget
     *  \warning
     *  如果调用了该函数就不能调用clientLayout()函数了，因为m_pClientLayout指针已被释放
     * @param client
     */
    void setClientWidget(QWidget *client);

protected:
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

private:
    QWidget *m_pContainerWidget;
    QVBoxLayout *m_pContainerLayout;

    QWidget *m_pClientWidget;
    QVBoxLayout *m_pClientLayout;

};
#endif


#endif // MUCUSTOMWINDOW_H
