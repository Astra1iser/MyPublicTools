#pragma   once
#ifndef MYTOOLTIP_H
#define MYTOOLTIP_H

//如何使用这个悬浮提示
//首先获取字符大小
//QFontMetrics fontWidth(ui.DirPath->font()); //获取字体大小 ui.DirPath是QString字符串
//设置表格元素显示及省略号位置及其tips
//QString elideNote = fontWidth.elidedText(QStrbuffer, Qt::ElideMiddle, 420);	//设置省略号相关参数
//开始设置
//例子1:
//ui.FileAndDirListTable->setItem(rowcount,1,new QTableWidgetItem(elideNote));	//文本的过长省略号
//ui.FileAndDirListTable->item(rowcount,1)->setToolTip(ToolTipWrap(QStrbuffer,400));//setToolTip前就是要设置tooltip的窗体元素
//例子2:
//ui.DirPath->setText(elideNote);
//ui.DirPath->setToolTip(ToolTipWrap(QStrbuffer,390) );





/*
 如何使用提示悬浮窗函数
 对于所有可设置setToolTip()的窗体对象来说
 使用setToolTip(ToolTipWrap(提示的文字,悬浮宽度,fontMetrics()));
 即可,注意参数3一定是一个固定函数fontMetrics()
*/
QString ToolTipWrap(QString strSrc,int width, const QFontMetrics& fontsize);

#endif