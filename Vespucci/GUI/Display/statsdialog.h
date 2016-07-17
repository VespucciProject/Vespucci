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
    ~StatsDialog();
signals:
    void SetActionChecked(bool checked);
protected:
    void showEvent(QShowEvent *ev);
    void closeEvent(QCloseEvent *ev);

public slots:
    void MatrixSelectionChanged(QStringList matrix_keys);
    void MatrixToBeRemoved(QStringList matrix_keys);
    void DatasetToBeRemoved(QString name);

private slots:
    void on_calculatePushButton_clicked();

private:
    void ClearFields();
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
