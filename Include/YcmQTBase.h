#pragma   once
#include <YcmBase.h>
#include <QTInclude\MyToolTip\MyToolTip.h> //�Ż���������ʾʵ�ֺ���
//#include <QTInclude\MyQHeaderView\MyQHeaderView.h> //�Ż��ı����
//#include <QTInclude\MyQTTitleBar\MyTitleBar.h> //�����ı�����

using namespace Base;

//#define CCS2QS(x,...)	CS2QS(x,__VA_ARGS__)

//������QT�Ļ���������
namespace QTBase
{
	//�����˻���������


	//��ֹQString�е������ַ����������
	QString YString(const char* str);

	//��CString�ַ���ת��ΪQString�ַ���
	QString  CS2QS(CString cs, ...);

	//��QString�ַ���ת��ΪCString�ַ���
	CString  QS2CS(QString qs);


}