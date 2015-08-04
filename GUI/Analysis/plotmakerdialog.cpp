#include "GUI/Analysis/plotmakerdialog.h"
#include "ui_plotmakerdialog.h"

PlotMakerDialog::PlotMakerDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::PlotMakerDialog)
{
    ui->setupUi(this);
    x_spin_box_ = findChild<QSpinBox *>("xSpinBox");
    y_spin_box_ = findChild<QSpinBox *>("ySpinBox");
    mapping_combo_box_ = findChild<QComboBox *>("mappingComboBox");
    variables_combo_box_ = findChild<QComboBox *>("variablesComboBox");
    workspace = ws;
    dataset_ = workspace->DatasetAt(row);

    QStringList variable_items;
    if (dataset_->principal_components_calculated()){
        variable_items.append("Principal Components Scores");
        variable_items.append("Principal Components Loadings");
    }
    if (dataset_->partial_least_squares_calculated()){
        variable_items.append("PLS Scores:");
        variable_items.append("PLS Loadings");
    }
    if (dataset_->vertex_components_calculated()){
        variable_items.append("VCA Scores");
    }
    if (dataset_->meta){
        variable_items.append("Parent Dataset Indices");
    }
}

PlotMakerDialog::~PlotMakerDialog()
{
    delete ui;
}

void PlotMakerDialog::on_buttonBox_accepted()
{
    close();
}
