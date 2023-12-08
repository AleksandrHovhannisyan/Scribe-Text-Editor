/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[43];
    char stringdata0[826];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 10), // "toggleUndo"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 13), // "undoAvailable"
QT_MOC_LITERAL(4, 37, 10), // "toggleRedo"
QT_MOC_LITERAL(5, 48, 13), // "redoAvailable"
QT_MOC_LITERAL(6, 62, 16), // "toggleCopyAndCut"
QT_MOC_LITERAL(7, 79, 16), // "copyCutAvailable"
QT_MOC_LITERAL(8, 96, 23), // "updateTabAndWindowTitle"
QT_MOC_LITERAL(9, 120, 8), // "closeTab"
QT_MOC_LITERAL(10, 129, 7), // "Editor*"
QT_MOC_LITERAL(11, 137, 10), // "tabToClose"
QT_MOC_LITERAL(12, 148, 5), // "index"
QT_MOC_LITERAL(13, 154, 16), // "closeTabShortcut"
QT_MOC_LITERAL(14, 171, 10), // "informUser"
QT_MOC_LITERAL(15, 182, 5), // "title"
QT_MOC_LITERAL(16, 188, 7), // "message"
QT_MOC_LITERAL(17, 196, 13), // "reportMessage"
QT_MOC_LITERAL(18, 210, 20), // "on_currentTabChanged"
QT_MOC_LITERAL(19, 231, 19), // "on_languageSelected"
QT_MOC_LITERAL(20, 251, 8), // "QAction*"
QT_MOC_LITERAL(21, 260, 14), // "languageAction"
QT_MOC_LITERAL(22, 275, 22), // "on_actionNew_triggered"
QT_MOC_LITERAL(23, 298, 22), // "on_actionSaveTriggered"
QT_MOC_LITERAL(24, 321, 23), // "on_actionOpen_triggered"
QT_MOC_LITERAL(25, 345, 23), // "on_actionExit_triggered"
QT_MOC_LITERAL(26, 369, 23), // "on_actionUndo_triggered"
QT_MOC_LITERAL(27, 393, 22), // "on_actionCut_triggered"
QT_MOC_LITERAL(28, 416, 23), // "on_actionCopy_triggered"
QT_MOC_LITERAL(29, 440, 24), // "on_actionPaste_triggered"
QT_MOC_LITERAL(30, 465, 23), // "on_actionFind_triggered"
QT_MOC_LITERAL(31, 489, 24), // "on_actionGo_To_triggered"
QT_MOC_LITERAL(32, 514, 29), // "on_actionSelect_All_triggered"
QT_MOC_LITERAL(33, 544, 23), // "on_actionRedo_triggered"
QT_MOC_LITERAL(34, 568, 24), // "on_actionPrint_triggered"
QT_MOC_LITERAL(35, 593, 29), // "on_actionStatus_Bar_triggered"
QT_MOC_LITERAL(36, 623, 28), // "on_actionTime_Date_triggered"
QT_MOC_LITERAL(37, 652, 23), // "on_actionFont_triggered"
QT_MOC_LITERAL(38, 676, 30), // "on_actionAuto_Indent_triggered"
QT_MOC_LITERAL(39, 707, 28), // "on_actionWord_Wrap_triggered"
QT_MOC_LITERAL(40, 736, 27), // "on_actionTool_Bar_triggered"
QT_MOC_LITERAL(41, 764, 30), // "on_actionCompileRiscVTriggered"
QT_MOC_LITERAL(42, 795, 30) // "on_actionUploadToSCMBTriggered"

    },
    "MainWindow\0toggleUndo\0\0undoAvailable\0"
    "toggleRedo\0redoAvailable\0toggleCopyAndCut\0"
    "copyCutAvailable\0updateTabAndWindowTitle\0"
    "closeTab\0Editor*\0tabToClose\0index\0"
    "closeTabShortcut\0informUser\0title\0"
    "message\0reportMessage\0on_currentTabChanged\0"
    "on_languageSelected\0QAction*\0"
    "languageAction\0on_actionNew_triggered\0"
    "on_actionSaveTriggered\0on_actionOpen_triggered\0"
    "on_actionExit_triggered\0on_actionUndo_triggered\0"
    "on_actionCut_triggered\0on_actionCopy_triggered\0"
    "on_actionPaste_triggered\0"
    "on_actionFind_triggered\0"
    "on_actionGo_To_triggered\0"
    "on_actionSelect_All_triggered\0"
    "on_actionRedo_triggered\0"
    "on_actionPrint_triggered\0"
    "on_actionStatus_Bar_triggered\0"
    "on_actionTime_Date_triggered\0"
    "on_actionFont_triggered\0"
    "on_actionAuto_Indent_triggered\0"
    "on_actionWord_Wrap_triggered\0"
    "on_actionTool_Bar_triggered\0"
    "on_actionCompileRiscVTriggered\0"
    "on_actionUploadToSCMBTriggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  174,    2, 0x0a /* Public */,
       4,    1,  177,    2, 0x0a /* Public */,
       6,    1,  180,    2, 0x0a /* Public */,
       8,    0,  183,    2, 0x0a /* Public */,
       9,    1,  184,    2, 0x0a /* Public */,
       9,    1,  187,    2, 0x0a /* Public */,
      13,    0,  190,    2, 0x0a /* Public */,
      14,    2,  191,    2, 0x0a /* Public */,
      17,    0,  196,    2, 0x0a /* Public */,
      18,    1,  197,    2, 0x08 /* Private */,
      19,    1,  200,    2, 0x08 /* Private */,
      22,    0,  203,    2, 0x08 /* Private */,
      23,    0,  204,    2, 0x08 /* Private */,
      24,    0,  205,    2, 0x08 /* Private */,
      25,    0,  206,    2, 0x08 /* Private */,
      26,    0,  207,    2, 0x08 /* Private */,
      27,    0,  208,    2, 0x08 /* Private */,
      28,    0,  209,    2, 0x08 /* Private */,
      29,    0,  210,    2, 0x08 /* Private */,
      30,    0,  211,    2, 0x08 /* Private */,
      31,    0,  212,    2, 0x08 /* Private */,
      32,    0,  213,    2, 0x08 /* Private */,
      33,    0,  214,    2, 0x08 /* Private */,
      34,    0,  215,    2, 0x08 /* Private */,
      35,    0,  216,    2, 0x08 /* Private */,
      36,    0,  217,    2, 0x08 /* Private */,
      37,    0,  218,    2, 0x08 /* Private */,
      38,    0,  219,    2, 0x08 /* Private */,
      39,    0,  220,    2, 0x08 /* Private */,
      40,    0,  221,    2, 0x08 /* Private */,
      41,    0,  222,    2, 0x08 /* Private */,
      42,    0,  223,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void,
    QMetaType::Bool, 0x80000000 | 10,   11,
    QMetaType::Bool, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   15,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void,
    QMetaType::Bool,
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

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->toggleUndo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->toggleRedo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->toggleCopyAndCut((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->updateTabAndWindowTitle(); break;
        case 4: { bool _r = _t->closeTab((*reinterpret_cast< Editor*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->closeTab((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->closeTabShortcut(); break;
        case 7: _t->informUser((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 8: _t->reportMessage(); break;
        case 9: _t->on_currentTabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_languageSelected((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 11: _t->on_actionNew_triggered(); break;
        case 12: { bool _r = _t->on_actionSaveTriggered();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 13: _t->on_actionOpen_triggered(); break;
        case 14: _t->on_actionExit_triggered(); break;
        case 15: _t->on_actionUndo_triggered(); break;
        case 16: _t->on_actionCut_triggered(); break;
        case 17: _t->on_actionCopy_triggered(); break;
        case 18: _t->on_actionPaste_triggered(); break;
        case 19: _t->on_actionFind_triggered(); break;
        case 20: _t->on_actionGo_To_triggered(); break;
        case 21: _t->on_actionSelect_All_triggered(); break;
        case 22: _t->on_actionRedo_triggered(); break;
        case 23: _t->on_actionPrint_triggered(); break;
        case 24: _t->on_actionStatus_Bar_triggered(); break;
        case 25: _t->on_actionTime_Date_triggered(); break;
        case 26: _t->on_actionFont_triggered(); break;
        case 27: _t->on_actionAuto_Indent_triggered(); break;
        case 28: _t->on_actionWord_Wrap_triggered(); break;
        case 29: _t->on_actionTool_Bar_triggered(); break;
        case 30: _t->on_actionCompileRiscVTriggered(); break;
        case 31: _t->on_actionUploadToSCMBTriggered(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Editor* >(); break;
            }
            break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
