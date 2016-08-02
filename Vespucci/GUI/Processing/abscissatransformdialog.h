#ifndef ABSCISSATRANSFORMDIALOG_H
#define ABSCISSATRANSFORMDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class AbscissaTransformDialog;
}

class AbscissaTransformDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AbscissaTransformDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key);
    ~AbscissaTransformDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AbscissaTransformDialog *ui;
    QSharedPointer<VespucciWorkspace> workspace_;
    QSharedPointer<VespucciDataset> dataset_;

    double ParseInput(QString text) const;
    double TextToDouble(QString text) const;
};

#endif // ABSCISSATRANSFORMDIALOG_H
