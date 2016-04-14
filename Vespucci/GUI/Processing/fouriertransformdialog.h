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
    explicit FourierTransformDialog(QWidget *parent, VespucciWorkspace *ws, const QString &dataset_key);
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

    QDoubleSpinBox *end_double_spin_box_;
    QLabel *end_label_;
    QSpinBox *spectrum_spin_box_;
    QComboBox *type_combo_box_;
    QComboBox *weights_combo_box_;
    QLabel *weights_label_;
    QDoubleSpinBox *parameter_double_spin_box_;
    QLabel *parameter_label_;
    QCustomPlot *spectrum_custom_plot_;
    QLabel *sine_power_label_;
    QDoubleSpinBox *sine_power_double_spin_box_;
    QSpinBox *power_spin_box_;
    QLabel *value_label_;

    QSharedPointer<VespucciDataset> dataset_;
    VespucciWorkspace *workspace;

};

#endif // FOURIERTRANSFORMDIALOG_H
