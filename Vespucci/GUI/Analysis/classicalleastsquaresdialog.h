#ifndef CLASSICALLEASTSQUARESDIALOG_H
#define CLASSICALLEASTSQUARESDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class ClassicalLeastSquaresDialog;
}

class ClassicalLeastSquaresDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClassicalLeastSquaresDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key);
    explicit ClassicalLeastSquaresDialog(QSharedPointer<VespucciWorkspace> ws, const QStringList &dataset_keys);
    ~ClassicalLeastSquaresDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ClassicalLeastSquaresDialog *ui;
    QSharedPointer<VespucciWorkspace> workspace_;
    QSharedPointer<VespucciDataset> dataset_;
    QStringList dataset_keys_;
};

#endif // CLASSICALLEASTSQUARESDIALOG_H
