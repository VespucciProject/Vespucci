#ifndef ABSCISSATRANSFORMDIALOG_H
#define ABSCISSATRANSFORMDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class AbscissaTransformDialog;
}

class AbscissaTransformDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AbscissaTransformDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~AbscissaTransformDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AbscissaTransformDialog *ui;
    VespucciWorkspace *workspace;
    QSharedPointer<VespucciDataset> dataset_;
    QLineEdit *out_factor_edit_;
    QLineEdit *in_factor_edit_;
    QComboBox *out_units_combo_box_;
    QComboBox *in_units_combo_box_;
    QLineEdit *description_edit_;

    double ParseInput(QString text) const;
    double TextToDouble(QString text) const;
};

#endif // ABSCISSATRANSFORMDIALOG_H
