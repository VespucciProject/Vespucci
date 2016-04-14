#ifndef PEAKFINDINGDIALOG_H
#define PEAKFINDINGDIALOG_H

#include <QDialog>
#include "Data/Dataset/vespuccidataset.h"

namespace Ui {
class PeakFindingDialog;
}

class PeakFindingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PeakFindingDialog(QWidget *parent, VespucciWorkspace *ws, const QString &dataset_key);
    ~PeakFindingDialog();

private slots:
    void on_browseButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::PeakFindingDialog *ui;
    QSharedPointer<VespucciDataset> dataset_;
    VespucciWorkspace *workspace;
    QComboBox *method_combo_box_;
    QComboBox *format_combo_box_;
    QComboBox *gradient_combo_box_;
    QCheckBox *width_check_box_;
    QCheckBox *save_coefs_check_box_;
    QCheckBox *save_coef_plots_check_box_;
    QCheckBox *save_ridges_check_box_;
    QCheckBox *save_ridge_plots_check_box_;
    QLineEdit *directory_line_edit_;
    QSpinBox *scale_spin_box_;
    QSpinBox *search_spin_box_;
    QSpinBox *length_spin_box_;
    QSpinBox *gap_spin_box_;
    QDoubleSpinBox *noise_double_spin_box_;

};

#endif // PEAKFINDINGDIALOG_H
