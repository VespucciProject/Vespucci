#include "cropdialog.h"
#include "ui_cropdialog.h"

CropDialog::CropDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::CropDialog)
{
    ui->setupUi(this);
    workspace = ws;
    dataset_ = workspace->DatasetAt(row);

    x_min_box_ = this->findChild<QDoubleSpinBox*>("xMinDoubleSpinBox");
    x_max_box_ = this->findChild<QDoubleSpinBox*>("xMaxDoubleSpinBox");
    y_min_box_ = this->findChild<QDoubleSpinBox*>("yMinDoubleSpinBox");
    y_max_box_ = this->findChild<QDoubleSpinBox*>("yMaxDoubleSpinBox");

    QCPRange key_range = dataset_->KeyRange();
    QCPRange value_range = dataset_->ValueRange();

    x_min_box_->setMinimum(key_range.lower);
    x_min_box_->setMaximum(key_range.upper);
    x_max_box_->setMinimum(key_range.lower);
    x_max_box_->setMaximum(key_range.upper);

    y_min_box_->setMinimum(value_range.lower);
    y_min_box_->setMaximum(value_range.upper);
    y_max_box_->setMinimum(value_range.lower);
    y_max_box_->setMaximum(value_range.upper);

}


CropDialog::~CropDialog()
{
    delete ui;
}

void CropDialog::on_buttonBox_accepted()
{

    double x_min = x_min_box_->value();
    double x_max = x_max_box_->value();
    double y_min = y_min_box_->value();
    double y_max = y_max_box_->value();

    if ((x_max - x_min) <= 0 || (y_max - y_min <= 0)){
        QMessageBox::warning(this, "Error", "Minimum is larger than maximum!");
        return;
    }

    else{
        dataset_->CropSpectra(x_min, x_max, y_min, y_max);
    }


}
