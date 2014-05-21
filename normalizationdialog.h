#ifndef NORMALIZATIONDIALOG_H
#define NORMALIZATIONDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"
#include "specmap.h"

namespace Ui {
class NormalizationDialog;
}

class NormalizationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NormalizationDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~NormalizationDialog();

private slots:
    void on_buttonBox_accepted();
    
private:
    Ui::NormalizationDialog *ui;
    QComboBox *method_selector_;
    VespucciWorkspace *workspace;
    SpecMap *target_dataset_;
};

#endif // NORMALIZATIONDIALOG_H
