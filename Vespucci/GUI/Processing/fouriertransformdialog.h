#ifndef FOURIERTRANSFORMDIALOG_H
#define FOURIERTRANSFORMDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
namespace Ui {
class FourierTransformDialog;
}

class FourierTransformDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FourierTransformDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key);
    ~FourierTransformDialog();

private slots:
    void on_typeComboBox_currentIndexChanged(const QString &arg1);

    void on_weightsComboBox_currentIndexChanged(const QString &arg1);

    void on_previewPushButton_clicked();

    void on_powerSpinBox_valueChanged(int arg1);

    void on_weightsPushButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::FourierTransformDialog *ui;

    QSharedPointer<VespucciDataset> dataset_;
    QSharedPointer<VespucciWorkspace> workspace_;

};

#endif // FOURIERTRANSFORMDIALOG_H
