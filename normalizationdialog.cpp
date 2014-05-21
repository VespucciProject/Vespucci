#include "normalizationdialog.h"
#include "ui_normalizationdialog.h"

NormalizationDialog::NormalizationDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::NormalizationDialog)
{
    ui->setupUi(this);
    method_selector_ = this->findChild<QComboBox *>("comboBox");
    workspace = ws;
    target_dataset_ = workspace->DatasetAt(row);
}

NormalizationDialog::~NormalizationDialog()
{
    delete ui;
}

void NormalizationDialog::on_buttonBox_accepted()
{
    switch (method_selector_->currentIndex())
    {
    case 0:
        target_dataset_->MinMaxNormalization();
        break;
    case 1:
        target_dataset_->StandardNormalVariableNormalization();
        break;
    case 2:
        target_dataset_->UnitAreaNormalization();
        break;
    default:
        QMessageBox message_box;
        message_box.setText("No processing performed");
        message_box.exec();
    }
}
