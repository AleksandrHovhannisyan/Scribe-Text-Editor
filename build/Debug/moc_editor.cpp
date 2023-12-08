/****************************************************************************
** Meta object code from reading C++ file 'editor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/editor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'editor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Editor_t {
    QByteArrayData data[34];
    char stringdata0[418];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Editor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Editor_t qt_meta_stringdata_Editor = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Editor"
QT_MOC_LITERAL(1, 7, 15), // "findResultReady"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 7), // "message"
QT_MOC_LITERAL(4, 32, 15), // "gotoResultReady"
QT_MOC_LITERAL(5, 48, 16), // "wordCountChanged"
QT_MOC_LITERAL(6, 65, 5), // "words"
QT_MOC_LITERAL(7, 71, 16), // "charCountChanged"
QT_MOC_LITERAL(8, 88, 5), // "chars"
QT_MOC_LITERAL(9, 94, 16), // "lineCountChanged"
QT_MOC_LITERAL(10, 111, 7), // "current"
QT_MOC_LITERAL(11, 119, 5), // "total"
QT_MOC_LITERAL(12, 125, 18), // "columnCountChanged"
QT_MOC_LITERAL(13, 144, 3), // "col"
QT_MOC_LITERAL(14, 148, 19), // "fileContentsChanged"
QT_MOC_LITERAL(15, 168, 4), // "find"
QT_MOC_LITERAL(16, 173, 5), // "query"
QT_MOC_LITERAL(17, 179, 13), // "caseSensitive"
QT_MOC_LITERAL(18, 193, 10), // "wholeWords"
QT_MOC_LITERAL(19, 204, 7), // "replace"
QT_MOC_LITERAL(20, 212, 4), // "what"
QT_MOC_LITERAL(21, 217, 4), // "with"
QT_MOC_LITERAL(22, 222, 10), // "replaceAll"
QT_MOC_LITERAL(23, 233, 4), // "goTo"
QT_MOC_LITERAL(24, 238, 4), // "line"
QT_MOC_LITERAL(25, 243, 14), // "on_textChanged"
QT_MOC_LITERAL(26, 258, 25), // "updateLineNumberAreaWidth"
QT_MOC_LITERAL(27, 284, 24), // "on_cursorPositionChanged"
QT_MOC_LITERAL(28, 309, 20), // "redrawLineNumberArea"
QT_MOC_LITERAL(29, 330, 15), // "rectToBeRedrawn"
QT_MOC_LITERAL(30, 346, 27), // "numPixelsScrolledVertically"
QT_MOC_LITERAL(31, 374, 16), // "setUndoAvailable"
QT_MOC_LITERAL(32, 391, 9), // "available"
QT_MOC_LITERAL(33, 401, 16) // "setRedoAvailable"

    },
    "Editor\0findResultReady\0\0message\0"
    "gotoResultReady\0wordCountChanged\0words\0"
    "charCountChanged\0chars\0lineCountChanged\0"
    "current\0total\0columnCountChanged\0col\0"
    "fileContentsChanged\0find\0query\0"
    "caseSensitive\0wholeWords\0replace\0what\0"
    "with\0replaceAll\0goTo\0line\0on_textChanged\0"
    "updateLineNumberAreaWidth\0"
    "on_cursorPositionChanged\0redrawLineNumberArea\0"
    "rectToBeRedrawn\0numPixelsScrolledVertically\0"
    "setUndoAvailable\0available\0setRedoAvailable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Editor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x06 /* Public */,
       4,    1,  102,    2, 0x06 /* Public */,
       5,    1,  105,    2, 0x06 /* Public */,
       7,    1,  108,    2, 0x06 /* Public */,
       9,    2,  111,    2, 0x06 /* Public */,
      12,    1,  116,    2, 0x06 /* Public */,
      14,    0,  119,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    3,  120,    2, 0x0a /* Public */,
      19,    4,  127,    2, 0x0a /* Public */,
      22,    4,  136,    2, 0x0a /* Public */,
      23,    1,  145,    2, 0x0a /* Public */,
      25,    0,  148,    2, 0x08 /* Private */,
      26,    0,  149,    2, 0x08 /* Private */,
      27,    0,  150,    2, 0x08 /* Private */,
      28,    2,  151,    2, 0x08 /* Private */,
      31,    1,  156,    2, 0x08 /* Private */,
      33,    1,  159,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   10,   11,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Bool, QMetaType::QString, QMetaType::Bool, QMetaType::Bool,   16,   17,   18,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Bool,   20,   21,   17,   18,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Bool,   20,   21,   17,   18,
    QMetaType::Void, QMetaType::Int,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QRect, QMetaType::Int,   29,   30,
    QMetaType::Void, QMetaType::Bool,   32,
    QMetaType::Void, QMetaType::Bool,   32,

       0        // eod
};

void Editor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Editor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->findResultReady((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->gotoResultReady((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->wordCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->charCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->lineCountChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->columnCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->fileContentsChanged(); break;
        case 7: { bool _r = _t->find((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->replace((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 9: _t->replaceAll((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 10: _t->goTo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_textChanged(); break;
        case 12: _t->updateLineNumberAreaWidth(); break;
        case 13: _t->on_cursorPositionChanged(); break;
        case 14: _t->redrawLineNumberArea((*reinterpret_cast< const QRect(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->setUndoAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->setRedoAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Editor::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Editor::findResultReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Editor::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Editor::gotoResultReady)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Editor::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Editor::wordCountChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Editor::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Editor::charCountChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Editor::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Editor::lineCountChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Editor::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Editor::columnCountChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Editor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Editor::fileContentsChanged)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Editor::staticMetaObject = { {
    QMetaObject::SuperData::link<QPlainTextEdit::staticMetaObject>(),
    qt_meta_stringdata_Editor.data,
    qt_meta_data_Editor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Editor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Editor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Editor.stringdata0))
        return static_cast<void*>(this);
    return QPlainTextEdit::qt_metacast(_clname);
}

int Editor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPlainTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void Editor::findResultReady(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Editor::gotoResultReady(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Editor::wordCountChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Editor::charCountChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Editor::lineCountChanged(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Editor::columnCountChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Editor::fileContentsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
