#ifndef FOURIERTRANSFORMDIALOG_H
#define FOURIERTRANSFORMDIALOG_H

#include <QDialog>

namespace Ui {
class FourierTransformDialog;
}

class FourierTransformDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FourierTransformDialog(QWidget *parent = 0);
    ~FourierTransformDialog();

private:
    Ui::FourierTransformDialog *ui;
};

#endif // FOURIERTRANSFORMDIALOG_H
