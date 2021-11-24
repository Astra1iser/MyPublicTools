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
    char stringdata0[255];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyTitleBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyTitleBar_t qt_meta_stringdata_MyTitleBar = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MyTitleBar"
QT_MOC_LITERAL(1, 11, 16), // "slotTimerTimeOut"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 17), // "_SlotPlayArgsMenu"
QT_MOC_LITERAL(4, 47, 3), // "pos"
QT_MOC_LITERAL(5, 51, 18), // "onButtonMinClicked"
QT_MOC_LITERAL(6, 70, 18), // "onButtonMinPressed"
QT_MOC_LITERAL(7, 89, 18), // "onButtonMaxClicked"
QT_MOC_LITERAL(8, 108, 18), // "onButtonMaxPressed"
QT_MOC_LITERAL(9, 127, 22), // "onButtonRestoreClicked"
QT_MOC_LITERAL(10, 150, 22), // "onButtonRestorePressed"
QT_MOC_LITERAL(11, 173, 20), // "onButtonCloseClicked"
QT_MOC_LITERAL(12, 194, 20), // "onButtonClosePressed"
QT_MOC_LITERAL(13, 215, 11), // "onRollTitle"
QT_MOC_LITERAL(14, 227, 13), // "onIconClicked"
QT_MOC_LITERAL(15, 241, 13) // "onIconPressed"

    },
    "MyTitleBar\0slotTimerTimeOut\0\0"
    "_SlotPlayArgsMenu\0pos\0onButtonMinClicked\0"
    "onButtonMinPressed\0onButtonMaxClicked\0"
    "onButtonMaxPressed\0onButtonRestoreClicked\0"
    "onButtonRestorePressed\0onButtonCloseClicked\0"
    "onButtonClosePressed\0onRollTitle\0"
    "onIconClicked\0onIconPressed"
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
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    1,   80,    2, 0x08 /* Private */,
       5,    0,   83,    2, 0x08 /* Private */,
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

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    4,
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
    QMetaType::Void,

       0        // eod
};

void MyTitleBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyTitleBar *_t = static_cast<MyTitleBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotTimerTimeOut(); break;
        case 1: _t->_SlotPlayArgsMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 2: _t->onButtonMinClicked(); break;
        case 3: _t->onButtonMinPressed(); break;
        case 4: _t->onButtonMaxClicked(); break;
        case 5: _t->onButtonMaxPressed(); break;
        case 6: _t->onButtonRestoreClicked(); break;
        case 7: _t->onButtonRestorePressed(); break;
        case 8: _t->onButtonCloseClicked(); break;
        case 9: _t->onButtonClosePressed(); break;
        case 10: _t->onRollTitle(); break;
        case 11: _t->onIconClicked(); break;
        case 12: _t->onIconPressed(); break;
        default: ;
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
QT_END_MOC_NAMESPACE
