#ifndef THRESHOLDDIALOG_H
#define THRESHOLDDIALOG_H

#include <QDialog>
#include "Data/Dataset/vespuccidataset.h"


namespace Ui {
class ThresholdDialog;
}

class ThresholdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ThresholdDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~ThresholdDialog();

private slots:
    void on_buttonBox_accepted();

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::ThresholdDialog *ui;
    QSharedPointer<VespucciDataset> dataset_;
    VespucciWorkspace *workspace;
    QDoubleSpinBox *threshold_box_;
    QComboBox *method_box_;

};

#endif // THRESHOLDDIALOG_H
