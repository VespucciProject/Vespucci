#include "citationdialog.h"
#include "ui_citationdialog.h"

CitationDialog::CitationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CitationDialog)
{
    ui->setupUi(this);
}

CitationDialog::~CitationDialog()
{
    delete ui;
}

void CitationDialog::on_pushButton_clicked()
{
    this->close();
}
