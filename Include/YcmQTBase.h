#pragma   once
#include <YcmBase.h>
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