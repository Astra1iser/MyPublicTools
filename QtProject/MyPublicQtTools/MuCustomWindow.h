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
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result);
};




#endif // MUCUSTOMWINDOW_H
