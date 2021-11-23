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
    QByteArrayData data[11];
    char stringdata0[188];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyTitleBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyTitleBar_t qt_meta_stringdata_MyTitleBar = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MyTitleBar"
QT_MOC_LITERAL(1, 11, 18), // "onButtonMinClicked"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 18), // "onButtonMinPressed"
QT_MOC_LITERAL(4, 50, 18), // "onButtonMaxClicked"
QT_MOC_LITERAL(5, 69, 18), // "onButtonMaxPressed"
QT_MOC_LITERAL(6, 88, 22), // "onButtonRestoreClicked"
QT_MOC_LITERAL(7, 111, 22), // "onButtonRestorePressed"
QT_MOC_LITERAL(8, 134, 20), // "onButtonCloseClicked"
QT_MOC_LITERAL(9, 155, 20), // "onButtonClosePressed"
QT_MOC_LITERAL(10, 176, 11) // "onRollTitle"

    },
    "MyTitleBar\0onButtonMinClicked\0\0"
    "onButtonMinPressed\0onButtonMaxClicked\0"
    "onButtonMaxPressed\0onButtonRestoreClicked\0"
    "onButtonRestorePressed\0onButtonCloseClicked\0"
    "onButtonClosePressed\0onRollTitle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyTitleBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    0,   66,    2, 0x08 /* Private */,
      10,    0,   67,    2, 0x08 /* Private */,

 // slots: parameters
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
        case 0: _t->onButtonMinClicked(); break;
        case 1: _t->onButtonMinPressed(); break;
        case 2: _t->onButtonMaxClicked(); break;
        case 3: _t->onButtonMaxPressed(); break;
        case 4: _t->onButtonRestoreClicked(); break;
        case 5: _t->onButtonRestorePressed(); break;
        case 6: _t->onButtonCloseClicked(); break;
        case 7: _t->onButtonClosePressed(); break;
        case 8: _t->onRollTitle(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
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
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
