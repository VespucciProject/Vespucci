#include "GUI/Processing/booleanizedialog.h"
#include "ui_booleanizedialog.h"

BooleanizeDialog::BooleanizeDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::BooleanizeDialog)
{
    ui->setupUi(this);

    min_box_ = findChild<QDoubleSpinBox*>("minDoubleSpinBox");
    max_box_ = findChild<QDoubleSpinBox*>("maxDoubleSpinBox");
    oneify_box_ = findChild<QCheckBox*>("oneifyCheckBox");
    behavior_box_ = findChild<QComboBox*>("behaviorComboBox");
    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);
    //not encapsulated, but this is basically research code anyway...
    min_box_->setValue(dataset_->spectra_ptr()->min());
    max_box_->setValue(dataset_->spectra_ptr()->max());


}

BooleanizeDialog::~BooleanizeDialog()
{
    delete ui;
}

void BooleanizeDialog::on_behaviorComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Set Values out of Range to Maximum (clamp)")
        oneify_box_->setEnabled(false);
    else
        oneify_box_->setEnabled(true);


}

void BooleanizeDialog::on_buttonBox_accepted()
{
    double min = std::min(min_box_->value(), max_box_->value());
    double max = std::max(min_box_->value(), max_box_->value());
    bool oneify = oneify_box_->isChecked();

    if (behavior_box_->currentText() == "Set Values out of Range to Maximum (clamp)")
        dataset_->Clamp(min, max);
    else if (behavior_box_->currentText() == "Set Values out of Range to Zero")
        dataset_->Booleanize(min, max, true, oneify);
    else
        dataset_->Booleanize(min, max, false, oneify);

}

void BooleanizeDialog::on_buttonBox_rejected()
{
    dataset_.clear();
    close();
}
