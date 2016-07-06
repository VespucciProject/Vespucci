#include "GUI/Analysis/plotmakerdialog.h"
#include "ui_plotmakerdialog.h"
#include "GUI/Display/plotviewer.h"

PlotMakerDialog::PlotMakerDialog(MainWindow *parent, QSharedPointer<VespucciWorkspace> ws, const mat & data) :
    QDialog(parent),
    ui(new Ui::PlotMakerDialog),
    data_(data)
{
    ui->setupUi(this);
    workspace_ = ws;
    x_spin_box_ = findChild<QDoubleSpinBox *>("xDoubleSpinBox");
    y_spin_box_ = findChild<QDoubleSpinBox *>("yDoubleSpinBox");
    dimension_combo_box_ = findChild<QComboBox *>("dimensionComboBox");


}

PlotMakerDialog::~PlotMakerDialog()
{
    delete ui;
}

void PlotMakerDialog::on_buttonBox_accepted()
{

}
