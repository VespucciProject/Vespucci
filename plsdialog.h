#ifndef PLSDIALOG_H
#define PLSDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"

namespace Ui {
class PLSDialog;
}

class PLSDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PLSDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~PLSDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::PLSDialog *ui;
    VespucciWorkspace *workspace;
    QSpinBox *image_component_selector_;
    QSpinBox *components_selector_;
    QComboBox *color_selector_;
    QLineEdit *name_box_;
    QCheckBox *recalculate_box_;

    //index of the VespucciDataset object in the various lists
    int data_index_;

    //Pointer to the VespucciDataset object at data_index_
    QSharedPointer<VespucciDataset> data_;
};

#endif // PLSDIALOG_H
