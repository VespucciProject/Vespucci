#include "GUI/Processing/thresholddialog.h"
#include "GUI/Processing/ui_thresholddialog.h"

ThresholdDialog::ThresholdDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::ThresholdDialog)
{
    ui->setupUi(this);
    dataset_ = ws->DatasetAt(row);
    workspace = ws;
    threshold_box_ = findChild<QDoubleSpinBox*>("doubleSpinBox");
    method_box_ = findChild<QComboBox*>("comboBox");
}

ThresholdDialog::~ThresholdDialog()
{
    delete ui;
}

void ThresholdDialog::on_buttonBox_accepted()
{
    QString method = method_box_->currentText();
    double threshold = threshold_box_->value();
    try{
        if (method == "Reject Maximum Above Threshold")
            dataset_->RemoveClippedSpectra(threshold);
        else
            dataset_->RemoveFlatSpectra(threshold);
    }
    catch(exception e){
        workspace->main_window()->DisplayExceptionWarning(e);
    }
}

void ThresholdDialog::on_comboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Reject Maximum Above Threshold"){
        threshold_box_->setValue(100);
    }
    else{
        threshold_box_->setValue(64000);
    }
}
