#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class StatsDialog;
}

class StatsDialog : public QDialog
{
    Q_OBJECT

public:
    StatsDialog(MainWindow *parent, VespucciWorkspace *ws);
    void SetActiveMatrix(const vec &data);
    void SetActiveDataKeys(const QStringList &keys);
    ~StatsDialog();


private slots:
    void on_buttonBox_accepted();

private:
    double CalculateMedian();
    double CalculateStdDev();
    double CalculateMean();
    void GenerateHistogram();
    void UpdateDisplayData();
    void CalcualteCI();
    QCustomPlot *histogram_custom_plot_;
    QDoubleSpinBox *alpha_double_spin_box_;
    Ui::StatsDialog *ui;
    QLineEdit *min_line_edit_;
    QLineEdit *max_line_edit_;
    QLineEdit *median_line_edit_;
    QLineEdit *stddev_line_edit_;
    QLineEdit *mean_line_edit_;
    QStringList data_keys_;
    VespucciWorkspace *workspace;
};

#endif // STATSDIALOG_H
