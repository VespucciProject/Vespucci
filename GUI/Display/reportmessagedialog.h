#ifndef REPORTMESSAGEDIALOG_H
#define REPORTMESSAGEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPlainTextEdit>

namespace Ui {
class ReportMessageDialog;
}
class QLabel;
class QPlainTextEdit;
class ReportMessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportMessageDialog(QWidget *parent, const QString &title);
    void setLabel(const QString &new_label);
    void appendPlainText(const QString &text);
    ~ReportMessageDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ReportMessageDialog *ui;
    QPlainTextEdit *text_edit_;
    QLabel *label_;
};

#endif // REPORTMESSAGEDIALOG_H
