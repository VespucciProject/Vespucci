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
    explicit BooleanizeDialog(QWidget *parent, VespucciWorkspace *ws, const QString &dataset_key);
    ~BooleanizeDialog();

private slots:
    void on_behaviorComboBox_currentIndexChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::BooleanizeDialog *ui;


    QDoubleSpinBox *min_box_;
    QDoubleSpinBox *max_box_;
    QCheckBox *oneify_box_;
    QComboBox *behavior_box_;


    VespucciWorkspace *workspace;
    QSharedPointer<VespucciDataset> dataset_;
};

#endif // BOOLEANIZEDIALOG_H
