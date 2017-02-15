#include "textimportdialog.h"
#include "ui_textimportdialog.h"

TextImportDialog::TextImportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextImportDialog)
{
    ui->setupUi(this);
}

TextImportDialog::~TextImportDialog()
{
    delete ui;
}
