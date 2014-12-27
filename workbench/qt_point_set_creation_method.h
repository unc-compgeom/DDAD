#ifndef QT_POINT_SET_CREATION_METHOD_H
#define QT_POINT_SET_CREATION_METHOD_H

#include <QGroupBox>

namespace Ui {
class PointSetCreationMethod;
}

class PointSetCreationMethod : public QGroupBox
{
    Q_OBJECT

public:
    explicit PointSetCreationMethod(QWidget *parent = 0);
    ~PointSetCreationMethod();

signals:
    void CreatePointSet(const QVector<QVector3D>& data);

private slots:
    void on_method_dropdown_currentTextChanged(const QString &arg1);

    void on_file_button_choose_clicked();

    void on_file_button_generate_clicked();

private:
    Ui::PointSetCreationMethod *ui;
};

#endif // QT_POINT_SET_CREATION_METHOD_H
