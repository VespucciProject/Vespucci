#include "GUI/Display/reportmessagedialog.h"
#include "ui_reportmessagedialog.h"

ReportMessageDialog::ReportMessageDialog(QWidget *parent, const QString &title) :
    QDialog(parent),
    ui(new Ui::ReportMessageDialog)
{
    ui->setupUi(this);
    label_ = findChild<QLabel*>("label");
    plain_text_edit_ = findChild<QPlainTextEdit*>("plainTextEdit");
    setWindowTitle(title);
}

void ReportMessageDialog::setLabel(const QString &new_label)
{
    label_->setText(new_label);
}

void ReportMessageDialog::appendPlainText(const QString &text)
{
    plain_text_edit_->appendPlainText(text);
}



ReportMessageDialog::~ReportMessageDialog()
{
    delete ui;
}

void ReportMessageDialog::on_buttonBox_accepted()
{
    close();
}
