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
    explicit ClassicalLeastSquaresDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, QSharedPointer<AbstractDataAnalyzer> analyzer);
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
    QSharedPointer<AbstractDataAnalyzer> analyzer_;
    QStringList matrix_keys_;
};

#endif // CLASSICALLEASTSQUARESDIALOG_H
