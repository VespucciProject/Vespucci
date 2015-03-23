#ifndef SCRIPTDIALOG_H
#define SCRIPTDIALOG_H

#include <QDialog>
#include "Data/Dataset/vespuccidataset.h"

namespace Ui {
class ScriptDialog;
}

class ScriptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~ScriptDialog();

private slots:
    void on_buttonBox_accepted();
    
private:
    Ui::ScriptDialog *ui;
    QPlainTextEdit *code_box_;
    QPlainTextEdit *receive_box_;
    QPlainTextEdit *send_box_;
    QComboBox *interpreter_selector_;
    QSharedPointer<VespucciDataset> dataset_;

};

#endif // SCRIPTDIALOG_H
