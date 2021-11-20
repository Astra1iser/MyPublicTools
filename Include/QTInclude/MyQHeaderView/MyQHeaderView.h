#pragma once
#include <QHeaderView>
#include <QPainter>
#include <QMouseEvent>


using namespace std;

class MyQHeaderView : public QHeaderView
{
	Q_OBJECT

public:

	explicit MyQHeaderView(Qt::Orientation orientation, QWidget *parent = 0);

signals:

	void headCheckBoxToggled(bool checked);



protected:

	void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;

	void mousePressEvent(QMouseEvent *event);

	void mouseMoveEvent(QMouseEvent *event);

public slots:
	void stdate(bool type);

private:

	bool m_isOn;//是否选中

	QPoint m_mousePoint;//鼠标位置

	mutable QRect m_RectHeaderCheckBox;//复选框的位置

};