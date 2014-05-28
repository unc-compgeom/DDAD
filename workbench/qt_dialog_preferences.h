/*
 * This file is part of RationalCAD.
 *
 * RationalCAD is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * RationalCAD is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License along with RationalCAD. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-01-29
 * @brief Modal dialog that allows user to alter global configuration settings.
 */

#ifndef RC_QT_DIALOG_PREFERENCES_H
#define RC_QT_DIALOG_PREFERENCES_H

// Qt
#include <QDialog>

// RationalCAD
#include "common.h"
#include "config.h"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

private slots:
    void on_selector_color_minor_clicked();

    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_buttonBox_applied();
    void on_buttonBox_defaultsrestored();

    void on_selector_color_major_clicked();

private:
    Ui::PreferencesDialog *ui;
};

#endif // RC_QT_DIALOG_PREFERENCES_H
