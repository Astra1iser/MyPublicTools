#pragma   once
#include <YcmQTBase.h>
#include <YcmBase.cpp>



QString QTBase::YString(const char* str)
{
	return QString::fromLocal8Bit(str);
}


QString QTBase::CS2QS(CString cs, ...)
{
	CString var_str;

	va_list	ap;
	va_start(ap, cs);
	var_str.FormatV(cs, ap);
	va_end(ap);

	return  QString::fromWCharArray((LPCTSTR)var_str, var_str.GetLength());
}


CString QTBase::QS2CS(QString qs)
{
	return  qs.toStdString().c_str();
}
