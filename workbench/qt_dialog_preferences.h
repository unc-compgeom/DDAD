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

#ifndef WB_QT_DIALOG_PREFERENCES_H
#define WB_QT_DIALOG_PREFERENCES_H

// Qt
#include <QDialog>

// DDAD
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

#endif // WB_QT_DIALOG_PREFERENCES_H
