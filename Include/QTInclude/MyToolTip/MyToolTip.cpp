#pragma   once
#include "MyToolTip.h"

QString ToolTipWrap(QString strSrc,int width, const QFontMetrics& fontsize)
{ 
	QString result;
	QFontMetrics fm(fontsize);
	int textWidthInPxs = fm.width(strSrc);
	//fontMetrics();
	const int rear = strSrc.length();
	int pre = 0, vernier = 1;
	unsigned int pickUpWidthPxs = 0;
	QString pickUp;
	unsigned int curLen = 0;

	if (textWidthInPxs <= width)
	{
		result = strSrc;
		return result;
	}

	while (vernier <= rear)
	{
		curLen = vernier - pre;
		pickUp = strSrc.mid(pre, curLen);
		pickUpWidthPxs = fm.width(pickUp);
		if (pickUpWidthPxs >= width)
		{
			result += pickUp + "\n";  // ���뻻�з�������ʹ��<br/>��ǩ
			pre = vernier;
			pickUpWidthPxs = 0;
		}
		++vernier;
	}

	if (pickUpWidthPxs < width && !pickUp.isEmpty())
	{
		result += pickUp;
	}

	return result;
}
