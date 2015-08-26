#ifndef FOURIERTRANSFORMDIALOG_H
#define FOURIERTRANSFORMDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
namespace Ui {
class FourierTransformDialog;
}

class FourierTransformDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FourierTransformDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~FourierTransformDialog();

private:
    Ui::FourierTransformDialog *ui;
};

#endif // FOURIERTRANSFORMDIALOG_H
