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
    explicit ScriptDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key);
    ~ScriptDialog();

private slots:
    void on_buttonBox_accepted();
    
private:
    Ui::ScriptDialog *ui;
    QSharedPointer<VespucciDataset> dataset_;
    QSharedPointer<VespucciWorkspace> workspace_;

};

#endif // SCRIPTDIALOG_H
