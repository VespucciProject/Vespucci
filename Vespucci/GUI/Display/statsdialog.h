#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
class MainWindow;
namespace Ui {
class StatsDialog;
}

class StatsDialog : public QDialog
{
    Q_OBJECT

public:
    StatsDialog(MainWindow *parent, QSharedPointer<VespucciWorkspace> ws);
    void SetActiveMatrix(const vec &data);
    void SetActiveDataKeys(const QStringList &keys);
    ~StatsDialog();


private slots:
    void on_buttonBox_accepted();

    void on_calculatePushButton_clicked();

private:
    double CalculateMedian();
    double CalculateStdDev();
    double CalculateMean();
    void GenerateHistogram();
    void UpdateDisplayData();
    void CalculateCI();
    QCustomPlot *histogram_custom_plot_;
    QDoubleSpinBox *alpha_double_spin_box_;
    Ui::StatsDialog *ui;
    QLineEdit *min_line_edit_;
    QLineEdit *max_line_edit_;
    QLineEdit *median_line_edit_;
    QLineEdit *stddev_line_edit_;
    QLineEdit *mean_line_edit_;
    QLineEdit *confidence_line_edit_;
    QLabel *plottable_label_;
    QLabel *dimension_label_;
    QLabel *name_label_;
    QLabel *mappable_label_;
    QStringList data_keys_;
    QSharedPointer<VespucciWorkspace> workspace_;
};

#endif // STATSDIALOG_H
