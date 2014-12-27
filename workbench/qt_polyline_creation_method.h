#ifndef QT_POLYLINE_CREATION_METHOD_H
#define QT_POLYLINE_CREATION_METHOD_H

#include <QGroupBox>

namespace Ui {
class PolylineCreationMethod;
}

class PolylineCreationMethod : public QGroupBox
{
    Q_OBJECT

public:
    explicit PolylineCreationMethod(QWidget *parent = 0);
    ~PolylineCreationMethod();

private:
    Ui::PolylineCreationMethod *ui;
};

#endif // QT_POLYLINE_CREATION_METHOD_H
