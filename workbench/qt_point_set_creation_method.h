/*
 * This file is part of DDAD.
 *
 * DDAD is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * DDAD is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License along with DDAD. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef POINT_SET_CREATION_METHOD_H
#define POINT_SET_CREATION_METHOD_H

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
    void on_file_button_clicked();
    void on_generate_clicked();

private:
    Ui::PointSetCreationMethod *ui;
};

#endif // POINT_SET_CREATION_METHOD_H
