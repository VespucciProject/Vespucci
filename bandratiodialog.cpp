#include "bandratiodialog.h"
#include "ui_bandratiodialog.h"

BandRatioDialog::BandRatioDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BandRatioDialog)
{
    ui->setupUi(this);
}

BandRatioDialog::~BandRatioDialog()
{
    delete ui;
}
