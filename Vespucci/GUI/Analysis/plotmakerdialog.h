#ifndef PLOTMAKERDIALOG_H
#define PLOTMAKERDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
#include "GUI/Display/plotviewer.h"

namespace Ui {
class PlotMakerDialog;
}

class PlotMakerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlotMakerDialog(MainWindow *parent, VespucciWorkspace *ws, const mat &data);
    ~PlotMakerDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PlotMakerDialog *ui;
    const mat &data_;
    VespucciWorkspace *workspace;
    QDoubleSpinBox *x_spin_box_;
    QDoubleSpinBox *y_spin_box_;
    QComboBox *mapping_combo_box_;
    QComboBox *dimension_combo_box_;
};

#endif // PLOTMAKERDIALOG_H
