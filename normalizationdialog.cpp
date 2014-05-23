#include "normalizationdialog.h"
#include "ui_normalizationdialog.h"

NormalizationDialog::NormalizationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NormalizationDialog)
{
    ui->setupUi(this);
}

NormalizationDialog::~NormalizationDialog()
{
    delete ui;
}
