#ifndef CROPDIALOG_H
#define CROPDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"
#include "specmap.h"

namespace Ui {
class CropDialog;
}

class CropDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CropDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~CropDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CropDialog *ui;

    QDoubleSpinBox *x_min_box_;
    QDoubleSpinBox *x_max_box_;
    QDoubleSpinBox *y_min_box_;
    QDoubleSpinBox *y_max_box_;

    VespucciWorkspace *workspace;
    QSharedPointer<SpecMap> dataset_;
};

#endif // CROPDIALOG_H
