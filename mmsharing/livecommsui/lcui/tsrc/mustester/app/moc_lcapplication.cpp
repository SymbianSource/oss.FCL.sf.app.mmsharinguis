/****************************************************************************
** Meta object code from reading C++ file 'lcapplication.h'
**
** Created: Mon 14. Sep 11:48:03 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0-garden)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "lcapplication.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lcapplication.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0-garden. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LcHbApplication[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      30,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LcHbApplication[] = {
    "LcHbApplication\0\0handleQuit()\0quit()\0"
};

const QMetaObject LcHbApplication::staticMetaObject = {
    { &HbApplication::staticMetaObject, qt_meta_stringdata_LcHbApplication,
      qt_meta_data_LcHbApplication, 0 }
};

const QMetaObject *LcHbApplication::metaObject() const
{
    return &staticMetaObject;
}

void *LcHbApplication::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LcHbApplication))
        return static_cast<void*>(const_cast< LcHbApplication*>(this));
    return HbApplication::qt_metacast(_clname);
}

int LcHbApplication::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = HbApplication::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: handleQuit(); break;
        case 1: quit(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void LcHbApplication::handleQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
