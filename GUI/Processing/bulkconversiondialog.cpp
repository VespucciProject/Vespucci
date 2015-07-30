#include "GUI/Processing/bulkconversiondialog.h"
#include "ui_bulkconversiondialog.h"

BulkConversionDialog::BulkConversionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BulkConversionDialog)
{
    ui->setupUi(this);
}

BulkConversionDialog::~BulkConversionDialog()
{
    delete ui;
}
