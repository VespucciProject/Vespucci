#include "GUI/Processing/thresholddialog.h"
#include "ui_thresholddialog.h"

ThresholdDialog::ThresholdDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::ThresholdDialog)
{
    ui->setupUi(this);
    dataset_ = ws->GetDataset(dataset_key);
    workspace_ = ws;
}

ThresholdDialog::~ThresholdDialog()
{
    delete ui;
}

void ThresholdDialog::on_buttonBox_accepted()
{
    QString method = ui->methodComboBox->currentText();
    QString behavior = ui->behaviorComboBox->currentText();
    double threshold = ui->thresholdDoubleSpinBox->value();
    try{
        if (method == "Reject Maximum Above Threshold"){
            if (behavior == "Delete")
                dataset_->RemoveClippedSpectra(threshold);
            if (behavior == "Zero")
                dataset_->ZeroClippedSpectra(threshold);
        }
        if (method == "Reject Minimum Below Threshold"){
            if (behavior == "Delete")
                dataset_->RemoveFlatSpectra(threshold);
            if (behavior == "Zero")
                dataset_->ZeroFlatSpectra(threshold);
        }
    }
    catch(exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
    }
}

void ThresholdDialog::on_comboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Reject Maximum Above Threshold"){
        ui->thresholdDoubleSpinBox->setValue(100);
    }
    else{
        ui->thresholdDoubleSpinBox->setValue(64000);
    }
}
