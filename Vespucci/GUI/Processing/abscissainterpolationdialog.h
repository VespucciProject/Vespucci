#ifndef ABSCISSAINTERPOLATIONDIALOG_H
#define ABSCISSAINTERPOLATIONDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
namespace Ui {
class AbscissaInterpolationDialog;
}

class AbscissaInterpolationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AbscissaInterpolationDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key);
    ~AbscissaInterpolationDialog();

private slots:
    void on_buttonBox_accepted();

    void on_sourceComboBox_currentIndexChanged(const QString &arg1);

    void on_methodComboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::AbscissaInterpolationDialog *ui;
    QSharedPointer<VespucciDataset> dataset_;
    QSharedPointer<VespucciWorkspace> workspace_;
};

#endif // ABSCISSAINTERPOLATIONDIALOG_H
