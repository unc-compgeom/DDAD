/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2012-05-23
 * @brief Modal dialog that displays program information (e.g. build date).
 */

#ifndef RC_QT_DIALOG_ABOUT_H
#define RC_QT_DIALOG_ABOUT_H

// Qt
#include <QtGui>
#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();
    void addBuildDate();

private:
    Ui::AboutDialog *ui;
};

#endif // RC_QT_DIALOG_ABOUT_H
