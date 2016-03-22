#ifndef CLASSICALLEASTSQUARESDIALOG_H
#define CLASSICALLEASTSQUARESDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class ClassicalLeastSquaresDialog;
}

class ClassicalLeastSquaresDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClassicalLeastSquaresDialog(QWidget *parent, VespucciWorkspace *ws, const QModelIndex &dataset_index);
    ~ClassicalLeastSquaresDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ClassicalLeastSquaresDialog *ui;
    QComboBox *reference_combo_box_;
    QLineEdit *name_line_edit_;
    VespucciWorkspace *workspace;
    QSharedPointer<VespucciDataset> dataset_;
};

#endif // CLASSICALLEASTSQUARESDIALOG_H
