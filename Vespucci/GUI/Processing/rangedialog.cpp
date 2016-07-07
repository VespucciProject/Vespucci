#include "GUI/Processing/rangedialog.h"
#include "ui_rangedialog.h"

///
/// \brief RangeDialog::RangeDialog
/// \param parent
/// \param min
/// \param max
///
RangeDialog::RangeDialog(QWidget *parent, double min, double max) :
    QDialog(parent),
    ui(new Ui::RangeDialog)
{
    ui->setupUi(this);
    min_ = min;
    max_ = max;
    QString range_label_text = QString::number(min) + " - " + QString::number(max);
    ui->rangeLabel->setText(range_label_text);
    ui->minBox->setRange(min, max);
    ui->maxBox->setRange(min, max);
    is_accepted_ = false;

}

RangeDialog::~RangeDialog()
{
    delete ui;
}

void RangeDialog::on_buttonBox_accepted()
{
    min_ = ui->minBox->value();
    max_ = ui->maxBox->value();
    is_accepted_ = true;
    emit DialogAccepted(min_, max_);
}

void RangeDialog::GetRange(double &min, double &max){
    min = min_;
    max = max_;
}

bool RangeDialog::is_accepted()
{
    return is_accepted_;
}


