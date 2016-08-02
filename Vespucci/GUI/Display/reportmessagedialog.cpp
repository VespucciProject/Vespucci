#include "GUI/Display/reportmessagedialog.h"
#include "ui_reportmessagedialog.h"

ReportMessageDialog::ReportMessageDialog(QWidget *parent, const QString &title) :
    QDialog(parent),
    ui(new Ui::ReportMessageDialog)
{
    ui->setupUi(this);
    setWindowTitle(title);
}

void ReportMessageDialog::setLabel(const QString &new_label)
{
    ui->label->setText(new_label);
}

void ReportMessageDialog::appendPlainText(const QString &text)
{
    ui->plainTextEdit->appendPlainText(text);
}



ReportMessageDialog::~ReportMessageDialog()
{
    delete ui;
}

void ReportMessageDialog::on_buttonBox_accepted()
{
    close();
}
