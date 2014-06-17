#ifndef BASELINEDIALOG_H
#define BASELINEDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"
class VespucciWorkspace;
class SpecMap;
namespace Ui {
class BaselineDialog;
}

class BaselineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BaselineDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~BaselineDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::BaselineDialog *ui;
    QSharedPointer<SpecMap> dataset_;
    VespucciWorkspace *workspace;
    QSpinBox *iterations_box_;
    QComboBox *method_box_;
    QSpinBox *window_size_box_;
};

#endif // BASELINEDIALOG_H
