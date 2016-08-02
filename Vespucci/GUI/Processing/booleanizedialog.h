#ifndef BOOLEANIZEDIALOG_H
#define BOOLEANIZEDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
#include "Data/Dataset/vespuccidataset.h"

namespace Ui {
class BooleanizeDialog;
}

class BooleanizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BooleanizeDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key);
    ~BooleanizeDialog();

private slots:
    void on_behaviorComboBox_currentIndexChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::BooleanizeDialog *ui;

    QSharedPointer<VespucciWorkspace> workspace_;
    QSharedPointer<VespucciDataset> dataset_;
};

#endif // BOOLEANIZEDIALOG_H
