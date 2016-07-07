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
    Ui::StatsDialog *ui;
    QStringList data_keys_;
    QSharedPointer<VespucciWorkspace> workspace_;
};

#endif // STATSDIALOG_H
