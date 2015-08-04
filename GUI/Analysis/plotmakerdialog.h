#ifndef PLOTMAKERDIALOG_H
#define PLOTMAKERDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class PlotMakerDialog;
}

class PlotMakerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlotMakerDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~PlotMakerDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PlotMakerDialog *ui;
    QSharedPointer<VespucciDataset> dataset_;
    VespucciWorkspace *workspace;
    QSpinBox *x_spin_box_;
    QSpinBox *y_spin_box_;
    QComboBox *mapping_combo_box_;
    QComboBox *variables_combo_box_;
};

#endif // PLOTMAKERDIALOG_H
