#ifndef TEXTIMPORTDIALOG_H
#define TEXTIMPORTDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
#include <QSharedPointer>
namespace Ui {
class TextImportDialog;
}

class TextImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextImportDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws);
    ~TextImportDialog();

private slots:
    void on_browseButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::TextImportDialog *ui;
    QSharedPointer<VespucciWorkspace> workspace_;
};

#endif // TEXTIMPORTDIALOG_H
