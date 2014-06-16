#include "/branches/develop/baselinedialog.h"
#include "ui_baselinedialog.h"

BaselineDialog::BaselineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BaselineDialog)
{
    ui->setupUi(this);
}

BaselineDialog::~BaselineDialog()
{
    delete ui;
}
