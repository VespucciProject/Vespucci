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
    explicit PeakFindingDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~PeakFindingDialog();

private slots:
    void on_browseButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::PeakFindingDialog *ui;
    QSharedPointer<VespucciDataset> dataset_;
    VespucciWorkspace *workspace;
    QComboBox *method_box_;
    QComboBox *format_box_;
    QComboBox *gradient_box_;
    QCheckBox *width_box_;
    QCheckBox *save_coefs_box_;
    QCheckBox *save_coef_plots_box_;
    QCheckBox *save_ridges_box_;
    QCheckBox *save_ridge_plots_box_;
    QLineEdit *directory_line_edit_;
    QSpinBox *scale_box_;
    QSpinBox *search_box_;
    QSpinBox *length_box_;
    QSpinBox *gap_box_;
    QDoubleSpinBox *noise_box_;

};

#endif // PEAKFINDINGDIALOG_H
