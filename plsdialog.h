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

private:
    Ui::PLSDialog *ui;
    VespucciWorkspace *workspace;
    QSpinBox *image_component_selector_;
    QSpinBox *components_selector_;
    QComboBox *color_selector_;
    QCheckBox *negative_score_box_;
    QLineEdit *name_box_;
    QCheckBox *recalculate_box_;

    //index of the SpecMap object in the various lists
    int data_index_;

    //Pointer to the SpecMap object at data_index_
    QSharedPointer<SpecMap> data_;
};

#endif // PLSDIALOG_H
