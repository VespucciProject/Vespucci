#ifndef BINARIZE_DIALOG_H
#define BINARIZE_DIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
#include "Data/Dataset/vespuccidataset.h"

namespace Ui {
class BinarizeDialog;
}

class BinarizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BinarizeDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key);
    ~BinarizeDialog();

private slots:
    void on_behaviorComboBox_currentIndexChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::BinarizeDialog *ui;

    QSharedPointer<VespucciWorkspace> workspace_;
    QSharedPointer<VespucciDataset> dataset_;
};

#endif // BINARIZE_DIALOG_H
