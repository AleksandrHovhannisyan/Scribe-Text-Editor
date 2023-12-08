/****************************************************************************
** Meta object code from reading C++ file 'metricreporter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/metricreporter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'metricreporter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MetricReporter_t {
    QByteArrayData data[11];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MetricReporter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MetricReporter_t qt_meta_stringdata_MetricReporter = {
    {
QT_MOC_LITERAL(0, 0, 14), // "MetricReporter"
QT_MOC_LITERAL(1, 15, 15), // "updateWordCount"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 9), // "wordCount"
QT_MOC_LITERAL(4, 42, 15), // "updateCharCount"
QT_MOC_LITERAL(5, 58, 9), // "charCount"
QT_MOC_LITERAL(6, 68, 15), // "updateLineCount"
QT_MOC_LITERAL(7, 84, 7), // "current"
QT_MOC_LITERAL(8, 92, 5), // "total"
QT_MOC_LITERAL(9, 98, 17), // "updateColumnCount"
QT_MOC_LITERAL(10, 116, 11) // "columnCount"

    },
    "MetricReporter\0updateWordCount\0\0"
    "wordCount\0updateCharCount\0charCount\0"
    "updateLineCount\0current\0total\0"
    "updateColumnCount\0columnCount"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MetricReporter[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a /* Public */,
       4,    1,   37,    2, 0x0a /* Public */,
       6,    2,   40,    2, 0x0a /* Public */,
       9,    1,   45,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,
    QMetaType::Void, QMetaType::Int,   10,

       0        // eod
};

void MetricReporter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MetricReporter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateWordCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->updateCharCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->updateLineCount((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->updateColumnCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MetricReporter::staticMetaObject = { {
    QMetaObject::SuperData::link<QFrame::staticMetaObject>(),
    qt_meta_stringdata_MetricReporter.data,
    qt_meta_data_MetricReporter,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MetricReporter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MetricReporter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MetricReporter.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int MetricReporter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
