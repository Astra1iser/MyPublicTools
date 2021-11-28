/****************************************************************************
** Meta object code from reading C++ file 'MyTitleBar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../MyTitleBar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MyTitleBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MyTitleBar_t {
    QByteArrayData data[16];
    char stringdata0[260];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyTitleBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyTitleBar_t qt_meta_stringdata_MyTitleBar = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MyTitleBar"
QT_MOC_LITERAL(1, 11, 16), // "monitorMessage_1"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 16), // "slotTimerTimeOut"
QT_MOC_LITERAL(4, 46, 16), // "slotPlayArgsMenu"
QT_MOC_LITERAL(5, 63, 3), // "pos"
QT_MOC_LITERAL(6, 67, 16), // "CorrectionWindow"
QT_MOC_LITERAL(7, 84, 11), // "onRollTitle"
QT_MOC_LITERAL(8, 96, 18), // "onButtonMinClicked"
QT_MOC_LITERAL(9, 115, 18), // "onButtonMinPressed"
QT_MOC_LITERAL(10, 134, 18), // "onButtonMaxClicked"
QT_MOC_LITERAL(11, 153, 18), // "onButtonMaxPressed"
QT_MOC_LITERAL(12, 172, 22), // "onButtonRestoreClicked"
QT_MOC_LITERAL(13, 195, 22), // "onButtonRestorePressed"
QT_MOC_LITERAL(14, 218, 20), // "onButtonCloseClicked"
QT_MOC_LITERAL(15, 239, 20) // "onButtonClosePressed"

    },
    "MyTitleBar\0monitorMessage_1\0\0"
    "slotTimerTimeOut\0slotPlayArgsMenu\0pos\0"
    "CorrectionWindow\0onRollTitle\0"
    "onButtonMinClicked\0onButtonMinPressed\0"
    "onButtonMaxClicked\0onButtonMaxPressed\0"
    "onButtonRestoreClicked\0onButtonRestorePressed\0"
    "onButtonCloseClicked\0onButtonClosePressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyTitleBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   80,    2, 0x08 /* Private */,
       4,    1,   81,    2, 0x08 /* Private */,
       6,    0,   84,    2, 0x08 /* Private */,
       7,    0,   85,    2, 0x08 /* Private */,
       8,    0,   86,    2, 0x08 /* Private */,
       9,    0,   87,    2, 0x08 /* Private */,
      10,    0,   88,    2, 0x08 /* Private */,
      11,    0,   89,    2, 0x08 /* Private */,
      12,    0,   90,    2, 0x08 /* Private */,
      13,    0,   91,    2, 0x08 /* Private */,
      14,    0,   92,    2, 0x08 /* Private */,
      15,    0,   93,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MyTitleBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyTitleBar *_t = static_cast<MyTitleBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->monitorMessage_1(); break;
        case 1: _t->slotTimerTimeOut(); break;
        case 2: _t->slotPlayArgsMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 3: _t->CorrectionWindow(); break;
        case 4: _t->onRollTitle(); break;
        case 5: _t->onButtonMinClicked(); break;
        case 6: _t->onButtonMinPressed(); break;
        case 7: _t->onButtonMaxClicked(); break;
        case 8: _t->onButtonMaxPressed(); break;
        case 9: _t->onButtonRestoreClicked(); break;
        case 10: _t->onButtonRestorePressed(); break;
        case 11: _t->onButtonCloseClicked(); break;
        case 12: _t->onButtonClosePressed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MyTitleBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyTitleBar::monitorMessage_1)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MyTitleBar::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MyTitleBar.data,
      qt_meta_data_MyTitleBar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyTitleBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyTitleBar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyTitleBar.stringdata0))
        return static_cast<void*>(const_cast< MyTitleBar*>(this));
    return QDialog::qt_metacast(_clname);
}

int MyTitleBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void MyTitleBar::monitorMessage_1()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
