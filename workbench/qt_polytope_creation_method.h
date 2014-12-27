#ifndef QT_POLYTOPE_CREATION_METHOD_H
#define QT_POLYTOPE_CREATION_METHOD_H

#include <QGroupBox>

namespace Ui {
class PolytopeCreationMethod;
}

class PolytopeCreationMethod : public QGroupBox
{
    Q_OBJECT

public:
    explicit PolytopeCreationMethod(QWidget *parent = 0);
    ~PolytopeCreationMethod();

private:
    Ui::PolytopeCreationMethod *ui;
};

#endif // QT_POLYTOPE_CREATION_METHOD_H
