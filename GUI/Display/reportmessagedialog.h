#ifndef REPORTMESSAGEDIALOG_H
#define REPORTMESSAGEDIALOG_H

#include <QDialog>

namespace Ui {
class ReportMessageDialog;
}

class ReportMessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportMessageDialog(QWidget *parent = 0, const QString &title);
    setLabel(const QString &new_label);
    appendPlainText(const QString &text);
    ~ReportMessageDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ReportMessageDialog *ui;
    QPlainTextEdit *text_edit_;
    QLabel *label_;
};

#endif // REPORTMESSAGEDIALOG_H
