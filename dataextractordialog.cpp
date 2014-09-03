#include "dataextractordialog.h"
#include "ui_dataextractordialog.h"

DataExtractorDialog::DataExtractorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataExtractorDialog)
{
    ui->setupUi(this);
}

DataExtractorDialog::~DataExtractorDialog()
{
    delete ui;
}
