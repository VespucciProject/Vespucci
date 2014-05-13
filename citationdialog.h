#ifndef CITATIONDIALOG_H
#define CITATIONDIALOG_H

#include <QDialog>

namespace Ui {
class CitationDialog;
}

class CitationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CitationDialog(QWidget *parent = 0);
    ~CitationDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CitationDialog *ui;
};

#endif // CITATIONDIALOG_H
