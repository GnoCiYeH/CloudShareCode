/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Qt/CloudSharedCoding/mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "loginAllowed",
    "",
    "projInited",
    "returnHeart",
    "dataProgress",
    "close",
    "openCloudProj",
    "openProj",
    "newLocalProj",
    "openLocalProj",
    "projectItemPressedSlot",
    "QTreeWidgetItem*",
    "showSetting",
    "openProjFile",
    "newProFile",
    "deleteProFile",
    "newCloudProj",
    "saveLocalProj",
    "addLocalFile",
    "addFile",
    "file_path",
    "openFileAndAddTab",
    "get_SubDir_Under_Dir",
    "path",
    "QStringList&",
    "list",
    "get_SubFile_Under_SubDir",
    "tag",
    "is_contain_file_name",
    "file_name",
    "QList<std::shared_ptr<FileInfo>>",
    "ptr_vector",
    "loginState",
    "on_tabWidget_tabCloseRequested",
    "index",
    "selectencodingMode",
    "runProject",
    "stopProject",
    "debugProject",
    "cmdStdin",
    "gotoStackFrame",
    "QListWidgetItem*",
    "on_treeWidget_itemDoubleClicked",
    "item",
    "column"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[92];
    char stringdata0[11];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[12];
    char stringdata5[13];
    char stringdata6[6];
    char stringdata7[14];
    char stringdata8[9];
    char stringdata9[13];
    char stringdata10[14];
    char stringdata11[23];
    char stringdata12[17];
    char stringdata13[12];
    char stringdata14[13];
    char stringdata15[11];
    char stringdata16[14];
    char stringdata17[13];
    char stringdata18[14];
    char stringdata19[13];
    char stringdata20[8];
    char stringdata21[10];
    char stringdata22[18];
    char stringdata23[21];
    char stringdata24[5];
    char stringdata25[13];
    char stringdata26[5];
    char stringdata27[25];
    char stringdata28[4];
    char stringdata29[21];
    char stringdata30[10];
    char stringdata31[33];
    char stringdata32[11];
    char stringdata33[11];
    char stringdata34[31];
    char stringdata35[6];
    char stringdata36[19];
    char stringdata37[11];
    char stringdata38[12];
    char stringdata39[13];
    char stringdata40[9];
    char stringdata41[15];
    char stringdata42[17];
    char stringdata43[32];
    char stringdata44[5];
    char stringdata45[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 12),  // "loginAllowed"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 10),  // "projInited"
        QT_MOC_LITERAL(36, 11),  // "returnHeart"
        QT_MOC_LITERAL(48, 12),  // "dataProgress"
        QT_MOC_LITERAL(61, 5),  // "close"
        QT_MOC_LITERAL(67, 13),  // "openCloudProj"
        QT_MOC_LITERAL(81, 8),  // "openProj"
        QT_MOC_LITERAL(90, 12),  // "newLocalProj"
        QT_MOC_LITERAL(103, 13),  // "openLocalProj"
        QT_MOC_LITERAL(117, 22),  // "projectItemPressedSlot"
        QT_MOC_LITERAL(140, 16),  // "QTreeWidgetItem*"
        QT_MOC_LITERAL(157, 11),  // "showSetting"
        QT_MOC_LITERAL(169, 12),  // "openProjFile"
        QT_MOC_LITERAL(182, 10),  // "newProFile"
        QT_MOC_LITERAL(193, 13),  // "deleteProFile"
        QT_MOC_LITERAL(207, 12),  // "newCloudProj"
        QT_MOC_LITERAL(220, 13),  // "saveLocalProj"
        QT_MOC_LITERAL(234, 12),  // "addLocalFile"
        QT_MOC_LITERAL(247, 7),  // "addFile"
        QT_MOC_LITERAL(255, 9),  // "file_path"
        QT_MOC_LITERAL(265, 17),  // "openFileAndAddTab"
        QT_MOC_LITERAL(283, 20),  // "get_SubDir_Under_Dir"
        QT_MOC_LITERAL(304, 4),  // "path"
        QT_MOC_LITERAL(309, 12),  // "QStringList&"
        QT_MOC_LITERAL(322, 4),  // "list"
        QT_MOC_LITERAL(327, 24),  // "get_SubFile_Under_SubDir"
        QT_MOC_LITERAL(352, 3),  // "tag"
        QT_MOC_LITERAL(356, 20),  // "is_contain_file_name"
        QT_MOC_LITERAL(377, 9),  // "file_name"
        QT_MOC_LITERAL(387, 32),  // "QList<std::shared_ptr<FileInfo>>"
        QT_MOC_LITERAL(420, 10),  // "ptr_vector"
        QT_MOC_LITERAL(431, 10),  // "loginState"
        QT_MOC_LITERAL(442, 30),  // "on_tabWidget_tabCloseRequested"
        QT_MOC_LITERAL(473, 5),  // "index"
        QT_MOC_LITERAL(479, 18),  // "selectencodingMode"
        QT_MOC_LITERAL(498, 10),  // "runProject"
        QT_MOC_LITERAL(509, 11),  // "stopProject"
        QT_MOC_LITERAL(521, 12),  // "debugProject"
        QT_MOC_LITERAL(534, 8),  // "cmdStdin"
        QT_MOC_LITERAL(543, 14),  // "gotoStackFrame"
        QT_MOC_LITERAL(558, 16),  // "QListWidgetItem*"
        QT_MOC_LITERAL(575, 31),  // "on_treeWidget_itemDoubleClicked"
        QT_MOC_LITERAL(607, 4),  // "item"
        QT_MOC_LITERAL(612, 6)   // "column"
    },
    "MainWindow",
    "loginAllowed",
    "",
    "projInited",
    "returnHeart",
    "dataProgress",
    "close",
    "openCloudProj",
    "openProj",
    "newLocalProj",
    "openLocalProj",
    "projectItemPressedSlot",
    "QTreeWidgetItem*",
    "showSetting",
    "openProjFile",
    "newProFile",
    "deleteProFile",
    "newCloudProj",
    "saveLocalProj",
    "addLocalFile",
    "addFile",
    "file_path",
    "openFileAndAddTab",
    "get_SubDir_Under_Dir",
    "path",
    "QStringList&",
    "list",
    "get_SubFile_Under_SubDir",
    "tag",
    "is_contain_file_name",
    "file_name",
    "QList<std::shared_ptr<FileInfo>>",
    "ptr_vector",
    "loginState",
    "on_tabWidget_tabCloseRequested",
    "index",
    "selectencodingMode",
    "runProject",
    "stopProject",
    "debugProject",
    "cmdStdin",
    "gotoStackFrame",
    "QListWidgetItem*",
    "on_treeWidget_itemDoubleClicked",
    "item",
    "column"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  200,    2, 0x06,    1 /* Public */,
       3,    0,  201,    2, 0x06,    2 /* Public */,
       4,    0,  202,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,  203,    2, 0x0a,    4 /* Public */,
       6,    0,  204,    2, 0x0a,    5 /* Public */,
       7,    0,  205,    2, 0x0a,    6 /* Public */,
       8,    1,  206,    2, 0x0a,    7 /* Public */,
       9,    0,  209,    2, 0x0a,    9 /* Public */,
      10,    0,  210,    2, 0x0a,   10 /* Public */,
      11,    2,  211,    2, 0x0a,   11 /* Public */,
      13,    0,  216,    2, 0x0a,   14 /* Public */,
      14,    0,  217,    2, 0x0a,   15 /* Public */,
      15,    0,  218,    2, 0x0a,   16 /* Public */,
      16,    0,  219,    2, 0x0a,   17 /* Public */,
      17,    0,  220,    2, 0x0a,   18 /* Public */,
      18,    0,  221,    2, 0x0a,   19 /* Public */,
      19,    0,  222,    2, 0x0a,   20 /* Public */,
      20,    1,  223,    2, 0x0a,   21 /* Public */,
      22,    1,  226,    2, 0x0a,   23 /* Public */,
      23,    2,  229,    2, 0x0a,   25 /* Public */,
      27,    3,  234,    2, 0x0a,   28 /* Public */,
      29,    2,  241,    2, 0x0a,   32 /* Public */,
      33,    0,  246,    2, 0x0a,   35 /* Public */,
      34,    1,  247,    2, 0x08,   36 /* Private */,
      36,    0,  250,    2, 0x08,   38 /* Private */,
      37,    0,  251,    2, 0x08,   39 /* Private */,
      38,    0,  252,    2, 0x08,   40 /* Private */,
      39,    0,  253,    2, 0x08,   41 /* Private */,
      40,    3,  254,    2, 0x08,   42 /* Private */,
      41,    1,  261,    2, 0x08,   46 /* Private */,
      43,    2,  264,    2, 0x08,   48 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12, QMetaType::Int,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 25,   24,   26,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 25, QMetaType::Int,   24,   26,   28,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 31,   30,   32,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Int,   35,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 42,    2,
    QMetaType::Void, 0x80000000 | 12, QMetaType::Int,   44,   45,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'loginAllowed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'projInited'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'returnHeart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'dataProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'close'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openCloudProj'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openProj'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'newLocalProj'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openLocalProj'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'projectItemPressedSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'showSetting'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openProjFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'newProFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'deleteProFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'newCloudProj'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'saveLocalProj'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addLocalFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addFile'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'openFileAndAddTab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'get_SubDir_Under_Dir'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QStringList &, std::false_type>,
        // method 'get_SubFile_Under_SubDir'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QStringList &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'is_contain_file_name'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVector<std::shared_ptr<FileInfo>>, std::false_type>,
        // method 'loginState'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'on_tabWidget_tabCloseRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'selectencodingMode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'runProject'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopProject'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'debugProject'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'cmdStdin'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'gotoStackFrame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'on_treeWidget_itemDoubleClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTreeWidgetItem *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->loginAllowed(); break;
        case 1: _t->projInited(); break;
        case 2: _t->returnHeart(); break;
        case 3: _t->dataProgress(); break;
        case 4: _t->close(); break;
        case 5: _t->openCloudProj(); break;
        case 6: _t->openProj((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->newLocalProj(); break;
        case 8: _t->openLocalProj(); break;
        case 9: _t->projectItemPressedSlot((*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 10: _t->showSetting(); break;
        case 11: _t->openProjFile(); break;
        case 12: _t->newProFile(); break;
        case 13: _t->deleteProFile(); break;
        case 14: _t->newCloudProj(); break;
        case 15: _t->saveLocalProj(); break;
        case 16: _t->addLocalFile(); break;
        case 17: { bool _r = _t->addFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 18: _t->openFileAndAddTab((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 19: { bool _r = _t->get_SubDir_Under_Dir((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QStringList&>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 20: { bool _r = _t->get_SubFile_Under_SubDir((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QStringList&>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 21: { bool _r = _t->is_contain_file_name((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QList<std::shared_ptr<FileInfo>>>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 22: { bool _r = _t->loginState();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 23: _t->on_tabWidget_tabCloseRequested((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 24: _t->selectencodingMode(); break;
        case 25: _t->runProject(); break;
        case 26: _t->stopProject(); break;
        case 27: _t->debugProject(); break;
        case 28: _t->cmdStdin((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 29: _t->gotoStackFrame((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 30: _t->on_treeWidget_itemDoubleClicked((*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::loginAllowed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::projInited; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (_t _q_method = &MainWindow::returnHeart; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 31)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 31;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::loginAllowed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MainWindow::projInited()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MainWindow::returnHeart()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
