#include "statsdialog.h"
#include "ui_statsdialog.h"

StatsDialog::StatsDialog(QWidget *parent, vec data) :
    QDialog(parent),
    ui(new Ui::StatsDialog)
{
    ui->setupUi(this);
    min_line_edit_ = this->findChild<QLineEdit *>("minLineEdit");
    max_line_edit_ = this->findChild<QLineEdit *>("maxLineEdit");
    median_line_edit_ = this->findChild<QLineEdit *>("medLineEdit");
    stddev_line_edit_ = this->findChild<QLineEdit *>("stddevLineEdit");
    mean_line_edit_ = this->findChild<QLineEdit *>("meanLineEdit");

    //we find the median this way because there is an error in the stdlib in MinGW-w64, maybe?

    min_line_edit_->setText(QString::number(data.min()));
    max_line_edit_->setText(QString::number(data.max()));
    median_line_edit_->setText(QString::number(median(data)));
    stddev_line_edit_->setText(QString::number(stddev(data)));
    mean_line_edit_->setText(QString::number(mean(data)));
}

StatsDialog::~StatsDialog()
{
    delete ui;
}
