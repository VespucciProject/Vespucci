#ifndef CLASSICALLEASTSQUARESDIALOG_H
#define CLASSICALLEASTSQUARESDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
#include "GUI/Processing/matrixselectiondialog.h"

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

public slots:
    void MatrixSelected(QStringList keys);
private slots:
    void on_buttonBox_accepted();

    void on_selectPushButton_clicked();

private:
    Ui::ClassicalLeastSquaresDialog *ui;
    MatrixSelectionDialog *matrix_selection_dialog_;
    QSharedPointer<VespucciWorkspace> workspace_;
    QSharedPointer<VespucciDataset> dataset_;
    QStringList dataset_keys_;
    QStringList matrix_keys_;
};

#endif // CLASSICALLEASTSQUARESDIALOG_H
