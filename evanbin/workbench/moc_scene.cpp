/****************************************************************************
** Meta object code from reading C++ file 'scene.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../workbench/scene.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DDAD__SceneObserver_t {
    QByteArrayData data[27];
    char stringdata[405];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DDAD__SceneObserver_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DDAD__SceneObserver_t qt_meta_stringdata_DDAD__SceneObserver = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 18),
QT_MOC_LITERAL(2, 39, 0),
QT_MOC_LITERAL(3, 40, 12),
QT_MOC_LITERAL(4, 53, 12),
QT_MOC_LITERAL(5, 66, 12),
QT_MOC_LITERAL(6, 79, 19),
QT_MOC_LITERAL(7, 99, 5),
QT_MOC_LITERAL(8, 105, 21),
QT_MOC_LITERAL(9, 127, 5),
QT_MOC_LITERAL(10, 133, 19),
QT_MOC_LITERAL(11, 153, 3),
QT_MOC_LITERAL(12, 157, 19),
QT_MOC_LITERAL(13, 177, 21),
QT_MOC_LITERAL(14, 199, 19),
QT_MOC_LITERAL(15, 219, 19),
QT_MOC_LITERAL(16, 239, 19),
QT_MOC_LITERAL(17, 259, 18),
QT_MOC_LITERAL(18, 278, 4),
QT_MOC_LITERAL(19, 283, 26),
QT_MOC_LITERAL(20, 310, 4),
QT_MOC_LITERAL(21, 315, 27),
QT_MOC_LITERAL(22, 343, 5),
QT_MOC_LITERAL(23, 349, 22),
QT_MOC_LITERAL(24, 372, 14),
QT_MOC_LITERAL(25, 387, 6),
QT_MOC_LITERAL(26, 394, 10)
    },
    "DDAD::SceneObserver\0UpdateVertexBuffer\0"
    "\0coverage_idx\0lighting_idx\0primtype_idx\0"
    "QVector<GL::Vertex>\0verts\0"
    "onBeginCreatePolyLine\0start\0"
    "onUpdateNewPolyLine\0cur\0onEndCreatePolyLine\0"
    "onBeginCreatePolytope\0onUpdateNewPolytope\0"
    "onEndCreatePolytope\0onCreateTerrainMesh\0"
    "QVector<QVector3D>\0data\0"
    "onUpdateSelectedObjectName\0name\0"
    "onUpdateSelectedObjectColor\0color\0"
    "onDeleteSelectedObject\0onSelectObject\0"
    "coords\0onDeselect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DDAD__SceneObserver[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   88,    2, 0x0a /* Public */,
      10,    1,   91,    2, 0x0a /* Public */,
      12,    0,   94,    2, 0x0a /* Public */,
      13,    2,   95,    2, 0x0a /* Public */,
      14,    1,  100,    2, 0x0a /* Public */,
      15,    0,  103,    2, 0x0a /* Public */,
      16,    1,  104,    2, 0x0a /* Public */,
      19,    1,  107,    2, 0x0a /* Public */,
      21,    1,  110,    2, 0x0a /* Public */,
      23,    0,  113,    2, 0x0a /* Public */,
      24,    1,  114,    2, 0x0a /* Public */,
      26,    0,  117,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::UInt, 0x80000000 | 6,    3,    4,    5,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::QVector2D,    9,
    QMetaType::Void, QMetaType::QVector2D,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVector2D, QMetaType::QVector2D,    9,   11,
    QMetaType::Void, QMetaType::QVector2D,   11,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QColor,   22,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVector2D,   25,
    QMetaType::Void,

       0        // eod
};

void DDAD::SceneObserver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SceneObserver *_t = static_cast<SceneObserver *>(_o);
        switch (_id) {
        case 0: _t->UpdateVertexBuffer((*reinterpret_cast< const quint32(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2])),(*reinterpret_cast< const quint32(*)>(_a[3])),(*reinterpret_cast< QVector<GL::Vertex>(*)>(_a[4]))); break;
        case 1: _t->onBeginCreatePolyLine((*reinterpret_cast< const QVector2D(*)>(_a[1]))); break;
        case 2: _t->onUpdateNewPolyLine((*reinterpret_cast< const QVector2D(*)>(_a[1]))); break;
        case 3: _t->onEndCreatePolyLine(); break;
        case 4: _t->onBeginCreatePolytope((*reinterpret_cast< const QVector2D(*)>(_a[1])),(*reinterpret_cast< const QVector2D(*)>(_a[2]))); break;
        case 5: _t->onUpdateNewPolytope((*reinterpret_cast< const QVector2D(*)>(_a[1]))); break;
        case 6: _t->onEndCreatePolytope(); break;
        case 7: _t->onCreateTerrainMesh((*reinterpret_cast< const QVector<QVector3D>(*)>(_a[1]))); break;
        case 8: _t->onUpdateSelectedObjectName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->onUpdateSelectedObjectColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 10: _t->onDeleteSelectedObject(); break;
        case 11: _t->onSelectObject((*reinterpret_cast< const QVector2D(*)>(_a[1]))); break;
        case 12: _t->onDeselect(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 3:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<GL::Vertex> >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QVector3D> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SceneObserver::*_t)(const quint32 , const quint32 , const quint32 , QVector<GL::Vertex> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SceneObserver::UpdateVertexBuffer)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject DDAD::SceneObserver::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DDAD__SceneObserver.data,
      qt_meta_data_DDAD__SceneObserver,  qt_static_metacall, 0, 0}
};


const QMetaObject *DDAD::SceneObserver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DDAD::SceneObserver::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DDAD__SceneObserver.stringdata))
        return static_cast<void*>(const_cast< SceneObserver*>(this));
    if (!strcmp(_clname, "Visual::IGeometryObserver"))
        return static_cast< Visual::IGeometryObserver*>(const_cast< SceneObserver*>(this));
    return QObject::qt_metacast(_clname);
}

int DDAD::SceneObserver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void DDAD::SceneObserver::UpdateVertexBuffer(const quint32 _t1, const quint32 _t2, const quint32 _t3, QVector<GL::Vertex> _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_DDAD__SceneManager_t {
    QByteArrayData data[8];
    char stringdata[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DDAD__SceneManager_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DDAD__SceneManager_t qt_meta_stringdata_DDAD__SceneManager = {
    {
QT_MOC_LITERAL(0, 0, 18),
QT_MOC_LITERAL(1, 19, 20),
QT_MOC_LITERAL(2, 40, 0),
QT_MOC_LITERAL(3, 41, 12),
QT_MOC_LITERAL(4, 54, 12),
QT_MOC_LITERAL(5, 67, 12),
QT_MOC_LITERAL(6, 80, 19),
QT_MOC_LITERAL(7, 100, 5)
    },
    "DDAD::SceneManager\0onUpdateVertexBuffer\0"
    "\0coverage_idx\0lighting_idx\0primtype_idx\0"
    "QVector<GL::Vertex>\0verts"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DDAD__SceneManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    4,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::UInt, 0x80000000 | 6,    3,    4,    5,    7,

       0        // eod
};

void DDAD::SceneManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SceneManager *_t = static_cast<SceneManager *>(_o);
        switch (_id) {
        case 0: _t->onUpdateVertexBuffer((*reinterpret_cast< const quint32(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2])),(*reinterpret_cast< const quint32(*)>(_a[3])),(*reinterpret_cast< QVector<GL::Vertex>(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 3:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<GL::Vertex> >(); break;
            }
            break;
        }
    }
}

const QMetaObject DDAD::SceneManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DDAD__SceneManager.data,
      qt_meta_data_DDAD__SceneManager,  qt_static_metacall, 0, 0}
};


const QMetaObject *DDAD::SceneManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DDAD::SceneManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DDAD__SceneManager.stringdata))
        return static_cast<void*>(const_cast< SceneManager*>(this));
    return QObject::qt_metacast(_clname);
}

int DDAD::SceneManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
