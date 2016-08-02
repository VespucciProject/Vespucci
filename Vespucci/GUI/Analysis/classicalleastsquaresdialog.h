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
    ~ClassicalLeastSquaresDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ClassicalLeastSquaresDialog *ui;
    QSharedPointer<VespucciWorkspace> workspace_;
    QSharedPointer<VespucciDataset> dataset_;
};

#endif // CLASSICALLEASTSQUARESDIALOG_H
