#include "rangedialog.h"
#include "ui_rangedialog.h"

RangeDialog::RangeDialog(QWidget *parent, double min, double max) :
    QDialog(parent),
    ui(new Ui::RangeDialog)
{
    ui->setupUi(this);
    min_ = min;
    max_ = max;
    range_label_ = this->findChild<QLabel*>("rangeLabel");
    min_box_ = this->findChild<QDoubleSpinBox*>("minBox");
    max_box_ = this->findChild<QDoubleSpinBox*>("maxBox");
    QString range_label_text = QString::number(min) + " - " + QString::number(max);
    range_label_->setText(range_label_text);
    min_box_->setRange(min, max);
    max_box_->setRange(min, max);
    is_accepted_ = false;

}

RangeDialog::~RangeDialog()
{
    delete ui;
}

void RangeDialog::on_buttonBox_accepted()
{
    min_ = min_box_->value();
    max_ = max_box_->value();
    is_accepted_ = true;
}

void RangeDialog::GetRange(double &min, double &max){
    min = min_;
    max = max_;
}
