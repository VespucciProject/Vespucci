#ifndef NORMALIZATIONDIALOG_H
#define NORMALIZATIONDIALOG_H

#include <QDialog>

namespace Ui {
class NormalizationDialog;
}

class NormalizationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NormalizationDialog(QWidget *parent = 0);
    ~NormalizationDialog();

private:
    Ui::NormalizationDialog *ui;
};

#endif // NORMALIZATIONDIALOG_H
