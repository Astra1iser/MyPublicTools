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
    QByteArrayData data[15];
    char stringdata0[290];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyTitleBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyTitleBar_t qt_meta_stringdata_MyTitleBar = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MyTitleBar"
QT_MOC_LITERAL(1, 11, 22), // "signalButtonMinClicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 26), // "signalButtonRestoreClicked"
QT_MOC_LITERAL(4, 62, 22), // "signalButtonMaxClicked"
QT_MOC_LITERAL(5, 85, 24), // "signalButtonCloseClicked"
QT_MOC_LITERAL(6, 110, 18), // "onButtonMinClicked"
QT_MOC_LITERAL(7, 129, 22), // "onButtonRestoreClicked"
QT_MOC_LITERAL(8, 152, 18), // "onButtonMaxClicked"
QT_MOC_LITERAL(9, 171, 20), // "onButtonCloseClicked"
QT_MOC_LITERAL(10, 192, 11), // "onRollTitle"
QT_MOC_LITERAL(11, 204, 19), // "onButtonMinClicked2"
QT_MOC_LITERAL(12, 224, 23), // "onButtonRestoreClicked2"
QT_MOC_LITERAL(13, 248, 19), // "onButtonMaxClicked2"
QT_MOC_LITERAL(14, 268, 21) // "onButtonCloseClicked2"

    },
    "MyTitleBar\0signalButtonMinClicked\0\0"
    "signalButtonRestoreClicked\0"
    "signalButtonMaxClicked\0signalButtonCloseClicked\0"
    "onButtonMinClicked\0onButtonRestoreClicked\0"
    "onButtonMaxClicked\0onButtonCloseClicked\0"
    "onRollTitle\0onButtonMinClicked2\0"
    "onButtonRestoreClicked2\0onButtonMaxClicked2\0"
    "onButtonCloseClicked2"
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
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    0,   80,    2, 0x06 /* Public */,
       4,    0,   81,    2, 0x06 /* Public */,
       5,    0,   82,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    0,   86,    2, 0x08 /* Private */,
      10,    0,   87,    2, 0x08 /* Private */,
      11,    0,   88,    2, 0x08 /* Private */,
      12,    0,   89,    2, 0x08 /* Private */,
      13,    0,   90,    2, 0x08 /* Private */,
      14,    0,   91,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

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
        case 0: _t->signalButtonMinClicked(); break;
        case 1: _t->signalButtonRestoreClicked(); break;
        case 2: _t->signalButtonMaxClicked(); break;
        case 3: _t->signalButtonCloseClicked(); break;
        case 4: _t->onButtonMinClicked(); break;
        case 5: _t->onButtonRestoreClicked(); break;
        case 6: _t->onButtonMaxClicked(); break;
        case 7: _t->onButtonCloseClicked(); break;
        case 8: _t->onRollTitle(); break;
        case 9: _t->onButtonMinClicked2(); break;
        case 10: _t->onButtonRestoreClicked2(); break;
        case 11: _t->onButtonMaxClicked2(); break;
        case 12: _t->onButtonCloseClicked2(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MyTitleBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyTitleBar::signalButtonMinClicked)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MyTitleBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyTitleBar::signalButtonRestoreClicked)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MyTitleBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyTitleBar::signalButtonMaxClicked)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MyTitleBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyTitleBar::signalButtonCloseClicked)) {
                *result = 3;
                return;
            }
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
void MyTitleBar::signalButtonMinClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void MyTitleBar::signalButtonRestoreClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void MyTitleBar::signalButtonMaxClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void MyTitleBar::signalButtonCloseClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
