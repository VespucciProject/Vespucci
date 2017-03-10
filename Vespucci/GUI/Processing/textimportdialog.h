#ifndef TEXTIMPORTDIALOG_H
#define TEXTIMPORTDIALOG_H

#include <QDialog>

namespace Ui {
class TextImportDialog;
}

class TextImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextImportDialog(QWidget *parent = 0);
    ~TextImportDialog();

private:
    Ui::TextImportDialog *ui;
};

#endif // TEXTIMPORTDIALOG_H
