#pragma   once
#include <YcmBase.h>
#include <QTInclude\MyToolTip\MyToolTip.h> //优化的悬浮提示实现函数
//#include <QTInclude\MyQHeaderView\MyQHeaderView.h> //优化的表格类
//#include <QTInclude\MyQTTitleBar\MyTitleBar.h> //独立的标题栏

using namespace Base;

//#define CCS2QS(x,...)	CS2QS(x,__VA_ARGS__)

//这里是QT的基础公共库
namespace QTBase
{
	//包含了基础公共库


	//防止QString中的中文字符乱码的问题
	QString YString(const char* str);

	//将CString字符串转换为QString字符串
	QString  CS2QS(CString cs, ...);

	//将QString字符串转换为CString字符串
	CString  QS2CS(QString qs);


}