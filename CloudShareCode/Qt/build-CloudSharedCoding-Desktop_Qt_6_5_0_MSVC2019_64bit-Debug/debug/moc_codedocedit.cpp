/****************************************************************************
** Meta object code from reading C++ file 'codedocedit.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Qt/CloudSharedCoding/codedocedit.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'codedocedit.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSCodeDocEditENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSCodeDocEditENDCLASS = QtMocHelpers::stringData(
    "CodeDocEdit",
    "updateLineNumerAreaWidth",
    "",
    "newBlockCount",
    "highlightCurrentLine",
    "updateLineNumberArea",
    "rect",
    "dy",
    "setBreakPoint",
    "QMouseEvent*"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSCodeDocEditENDCLASS_t {
    uint offsetsAndSizes[20];
    char stringdata0[12];
    char stringdata1[25];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[21];
    char stringdata5[21];
    char stringdata6[5];
    char stringdata7[3];
    char stringdata8[14];
    char stringdata9[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSCodeDocEditENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSCodeDocEditENDCLASS_t qt_meta_stringdata_CLASSCodeDocEditENDCLASS = {
    {
        QT_MOC_LITERAL(0, 11),  // "CodeDocEdit"
        QT_MOC_LITERAL(12, 24),  // "updateLineNumerAreaWidth"
        QT_MOC_LITERAL(37, 0),  // ""
        QT_MOC_LITERAL(38, 13),  // "newBlockCount"
        QT_MOC_LITERAL(52, 20),  // "highlightCurrentLine"
        QT_MOC_LITERAL(73, 20),  // "updateLineNumberArea"
        QT_MOC_LITERAL(94, 4),  // "rect"
        QT_MOC_LITERAL(99, 2),  // "dy"
        QT_MOC_LITERAL(102, 13),  // "setBreakPoint"
        QT_MOC_LITERAL(116, 12)   // "QMouseEvent*"
    },
    "CodeDocEdit",
    "updateLineNumerAreaWidth",
    "",
    "newBlockCount",
    "highlightCurrentLine",
    "updateLineNumberArea",
    "rect",
    "dy",
    "setBreakPoint",
    "QMouseEvent*"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSCodeDocEditENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x08,    1 /* Private */,
       4,    0,   41,    2, 0x08,    3 /* Private */,
       5,    2,   42,    2, 0x08,    4 /* Private */,
       8,    1,   47,    2, 0x08,    7 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QRect, QMetaType::Int,    6,    7,
    QMetaType::Void, 0x80000000 | 9,    2,

       0        // eod
};

Q_CONSTINIT const QMetaObject CodeDocEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QPlainTextEdit::staticMetaObject>(),
    qt_meta_stringdata_CLASSCodeDocEditENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSCodeDocEditENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSCodeDocEditENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CodeDocEdit, std::true_type>,
        // method 'updateLineNumerAreaWidth'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'highlightCurrentLine'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateLineNumberArea'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QRect &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setBreakPoint'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMouseEvent *, std::false_type>
    >,
    nullptr
} };

void CodeDocEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CodeDocEdit *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateLineNumerAreaWidth((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->highlightCurrentLine(); break;
        case 2: _t->updateLineNumberArea((*reinterpret_cast< std::add_pointer_t<QRect>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->setBreakPoint((*reinterpret_cast< std::add_pointer_t<QMouseEvent*>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *CodeDocEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CodeDocEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSCodeDocEditENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QPlainTextEdit::qt_metacast(_clname);
}

int CodeDocEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPlainTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSLineNumberAreaENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSLineNumberAreaENDCLASS = QtMocHelpers::stringData(
    "LineNumberArea",
    "clicked",
    "",
    "QMouseEvent*",
    "event"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSLineNumberAreaENDCLASS_t {
    uint offsetsAndSizes[10];
    char stringdata0[15];
    char stringdata1[8];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSLineNumberAreaENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSLineNumberAreaENDCLASS_t qt_meta_stringdata_CLASSLineNumberAreaENDCLASS = {
    {
        QT_MOC_LITERAL(0, 14),  // "LineNumberArea"
        QT_MOC_LITERAL(15, 7),  // "clicked"
        QT_MOC_LITERAL(23, 0),  // ""
        QT_MOC_LITERAL(24, 12),  // "QMouseEvent*"
        QT_MOC_LITERAL(37, 5)   // "event"
    },
    "LineNumberArea",
    "clicked",
    "",
    "QMouseEvent*",
    "event"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSLineNumberAreaENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject LineNumberArea::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSLineNumberAreaENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSLineNumberAreaENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSLineNumberAreaENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LineNumberArea, std::true_type>,
        // method 'clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMouseEvent *, std::false_type>
    >,
    nullptr
} };

void LineNumberArea::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LineNumberArea *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->clicked((*reinterpret_cast< std::add_pointer_t<QMouseEvent*>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LineNumberArea::*)(QMouseEvent * );
            if (_t _q_method = &LineNumberArea::clicked; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *LineNumberArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LineNumberArea::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSLineNumberAreaENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LineNumberArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void LineNumberArea::clicked(QMouseEvent * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
