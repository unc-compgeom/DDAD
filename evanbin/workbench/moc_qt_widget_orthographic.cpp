/****************************************************************************
** Meta object code from reading C++ file 'qt_widget_orthographic.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../workbench/qt_widget_orthographic.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qt_widget_orthographic.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_OrthographicWidget_t {
    QByteArrayData data[16];
    char stringdata[198];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OrthographicWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OrthographicWidget_t qt_meta_stringdata_OrthographicWidget = {
    {
QT_MOC_LITERAL(0, 0, 18),
QT_MOC_LITERAL(1, 19, 13),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 3),
QT_MOC_LITERAL(4, 38, 12),
QT_MOC_LITERAL(5, 51, 6),
QT_MOC_LITERAL(6, 58, 19),
QT_MOC_LITERAL(7, 78, 5),
QT_MOC_LITERAL(8, 84, 17),
QT_MOC_LITERAL(9, 102, 3),
QT_MOC_LITERAL(10, 106, 17),
QT_MOC_LITERAL(11, 124, 19),
QT_MOC_LITERAL(12, 144, 17),
QT_MOC_LITERAL(13, 162, 17),
QT_MOC_LITERAL(14, 180, 15),
QT_MOC_LITERAL(15, 196, 1)
    },
    "OrthographicWidget\0ChangeMessage\0\0msg\0"
    "SelectObject\0coords\0BeginCreatePolyLine\0"
    "start\0UpdateNewPolyLine\0cur\0"
    "EndCreatePolyLine\0BeginCreatePolytope\0"
    "UpdateNewPolytope\0EndCreatePolytope\0"
    "ShowContextMenu\0p"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OrthographicWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    1,   62,    2, 0x06 /* Public */,
       6,    1,   65,    2, 0x06 /* Public */,
       8,    1,   68,    2, 0x06 /* Public */,
      10,    0,   71,    2, 0x06 /* Public */,
      11,    2,   72,    2, 0x06 /* Public */,
      12,    1,   77,    2, 0x06 /* Public */,
      13,    0,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    1,   81,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QVector2D,    5,
    QMetaType::Void, QMetaType::QVector2D,    7,
    QMetaType::Void, QMetaType::QVector2D,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVector2D, QMetaType::QVector2D,    7,    9,
    QMetaType::Void, QMetaType::QVector2D,    9,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QPoint,   15,

       0        // eod
};

void OrthographicWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OrthographicWidget *_t = static_cast<OrthographicWidget *>(_o);
        switch (_id) {
        case 0: _t->ChangeMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->SelectObject((*reinterpret_cast< const QVector2D(*)>(_a[1]))); break;
        case 2: _t->BeginCreatePolyLine((*reinterpret_cast< const QVector2D(*)>(_a[1]))); break;
        case 3: _t->UpdateNewPolyLine((*reinterpret_cast< const QVector2D(*)>(_a[1]))); break;
        case 4: _t->EndCreatePolyLine(); break;
        case 5: _t->BeginCreatePolytope((*reinterpret_cast< const QVector2D(*)>(_a[1])),(*reinterpret_cast< const QVector2D(*)>(_a[2]))); break;
        case 6: _t->UpdateNewPolytope((*reinterpret_cast< const QVector2D(*)>(_a[1]))); break;
        case 7: _t->EndCreatePolytope(); break;
        case 8: _t->ShowContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (OrthographicWidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OrthographicWidget::ChangeMessage)) {
                *result = 0;
            }
        }
        {
            typedef void (OrthographicWidget::*_t)(const QVector2D & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OrthographicWidget::SelectObject)) {
                *result = 1;
            }
        }
        {
            typedef void (OrthographicWidget::*_t)(const QVector2D & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OrthographicWidget::BeginCreatePolyLine)) {
                *result = 2;
            }
        }
        {
            typedef void (OrthographicWidget::*_t)(const QVector2D & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OrthographicWidget::UpdateNewPolyLine)) {
                *result = 3;
            }
        }
        {
            typedef void (OrthographicWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OrthographicWidget::EndCreatePolyLine)) {
                *result = 4;
            }
        }
        {
            typedef void (OrthographicWidget::*_t)(const QVector2D & , const QVector2D & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OrthographicWidget::BeginCreatePolytope)) {
                *result = 5;
            }
        }
        {
            typedef void (OrthographicWidget::*_t)(const QVector2D & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OrthographicWidget::UpdateNewPolytope)) {
                *result = 6;
            }
        }
        {
            typedef void (OrthographicWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OrthographicWidget::EndCreatePolytope)) {
                *result = 7;
            }
        }
    }
}

const QMetaObject OrthographicWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_OrthographicWidget.data,
      qt_meta_data_OrthographicWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *OrthographicWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrthographicWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OrthographicWidget.stringdata))
        return static_cast<void*>(const_cast< OrthographicWidget*>(this));
    if (!strcmp(_clname, "QOpenGLFunctions_3_3_Core"))
        return static_cast< QOpenGLFunctions_3_3_Core*>(const_cast< OrthographicWidget*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int OrthographicWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void OrthographicWidget::ChangeMessage(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OrthographicWidget::SelectObject(const QVector2D & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void OrthographicWidget::BeginCreatePolyLine(const QVector2D & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void OrthographicWidget::UpdateNewPolyLine(const QVector2D & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void OrthographicWidget::EndCreatePolyLine()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void OrthographicWidget::BeginCreatePolytope(const QVector2D & _t1, const QVector2D & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void OrthographicWidget::UpdateNewPolytope(const QVector2D & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void OrthographicWidget::EndCreatePolytope()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}
QT_END_MOC_NAMESPACE
