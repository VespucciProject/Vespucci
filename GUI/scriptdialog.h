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
    QPlainTextEdit *code_plain_test_edit_;
    QPlainTextEdit *receive_plain_test_edit_;
    QPlainTextEdit *send_plain_test_edit_;
    QComboBox *interpreter_selector_combo_box_;
    QSharedPointer<VespucciDataset> dataset_;
    VespucciWorkspace *workspace;

};

#endif // SCRIPTDIALOG_H
