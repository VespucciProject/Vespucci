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
    explicit FourierTransformDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~FourierTransformDialog();

private slots:
    void on_typeComboBox_currentIndexChanged(const QString &arg1);

    void on_weightsComboBox_currentIndexChanged(const QString &arg1);

    void on_previewPushButton_clicked();

private:
    Ui::FourierTransformDialog *ui;

    QDoubleSpinBox *cut_double_spin_box_;
    QLabel *cut_label_;
    QSpinBox *spectrum_spin_box_;
    QComboBox *type_combo_box_;
    QComboBox *weights_combo_box_;
    QLabel *weights_label_;
    QDoubleSpinBox *parameter_double_spin_box_;
    QLabel *parameter_label_;
    QCheckBox *prediction_check_box_;
    QCustomPlot *spectrum_custom_plot_;

};

#endif // FOURIERTRANSFORMDIALOG_H
