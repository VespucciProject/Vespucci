#include "GUI/Processing/booleanizedialog.h"
#include "ui_booleanizedialog.h"

BooleanizeDialog::BooleanizeDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::BooleanizeDialog)
{
    ui->setupUi(this);

    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);
    //not encapsulated, but this is basically research code anyway...
    ui->minDoubleSpinBox->setValue(dataset_->spectra_ptr()->min());
    ui->maxDoubleSpinBox->setValue(dataset_->spectra_ptr()->max());


}

BooleanizeDialog::~BooleanizeDialog()
{
    delete ui;
}

void BooleanizeDialog::on_behaviorComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Set Values out of Range to Maximum (clamp)")
        ui->oneifyCheckBox->setEnabled(false);
    else
        ui->oneifyCheckBox->setEnabled(true);


}

void BooleanizeDialog::on_buttonBox_accepted()
{
    double min = std::min(ui->minDoubleSpinBox->value(), ui->maxDoubleSpinBox->value());
    double max = std::max(ui->minDoubleSpinBox->value(), ui->maxDoubleSpinBox->value());
    bool oneify = ui->oneifyCheckBox->isChecked();

    if (ui->behaviorComboBox->currentText() == "Set Values out of Range to Maximum (clamp)")
        dataset_->Clamp(min, max);
    else if (ui->behaviorComboBox->currentText() == "Set Values out of Range to Zero")
        dataset_->Booleanize(min, max, true, oneify);
    else
        dataset_->Booleanize(min, max, false, oneify);

}

void BooleanizeDialog::on_buttonBox_rejected()
{
    dataset_.clear();
    close();
}
