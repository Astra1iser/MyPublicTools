#pragma   once
#ifndef MYTOOLTIP_H
#define MYTOOLTIP_H

//���ʹ�����������ʾ
//���Ȼ�ȡ�ַ���С
//QFontMetrics fontWidth(ui.DirPath->font()); //��ȡ�����С ui.DirPath��QString�ַ���
//���ñ��Ԫ����ʾ��ʡ�Ժ�λ�ü���tips
//QString elideNote = fontWidth.elidedText(QStrbuffer, Qt::ElideMiddle, 420);	//����ʡ�Ժ���ز���
//��ʼ����
//����1:
//ui.FileAndDirListTable->setItem(rowcount,1,new QTableWidgetItem(elideNote));	//�ı��Ĺ���ʡ�Ժ�
//ui.FileAndDirListTable->item(rowcount,1)->setToolTip(ToolTipWrap(QStrbuffer,400));//setToolTipǰ����Ҫ����tooltip�Ĵ���Ԫ��
//����2:
//ui.DirPath->setText(elideNote);
//ui.DirPath->setToolTip(ToolTipWrap(QStrbuffer,390) );





/*
 ���ʹ����ʾ����������
 �������п�����setToolTip()�Ĵ��������˵
 ʹ��setToolTip(ToolTipWrap(��ʾ������,�������,fontMetrics()));
 ����,ע�����3һ����һ���̶�����fontMetrics()
*/
QString ToolTipWrap(QString strSrc,int width, const QFontMetrics& fontsize);

#endif