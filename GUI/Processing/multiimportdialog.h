#ifndef MULTIIMPORTDIALOG_H
#define MULTIIMPORTDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class MultiImportDialog;
}

class MultiImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MultiImportDialog(QWidget *parent, VespucciWorkspace * ws);
    ~MultiImportDialog();

private slots:
    void on_rowSpinBox_valueChanged(int arg1);

    void on_colSpinBox_valueChanged(int arg1);

    void on_addFilesPushButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::MultiImportDialog *ui;
    VespucciWorkspace *workspace;
    QTableWidget *filename_table_;
    QLabel *count_label_;
    QLineEdit *name_box_;
    QLineEdit *abscissa_label_box_;
    QLineEdit *ordinate_label_box_;

};

#endif // MULTIIMPORTDIALOG_H
