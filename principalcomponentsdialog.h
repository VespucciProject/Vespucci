#ifndef PRINCIPALCOMPONENTSDIALOG_H
#define PRINCIPALCOMPONENTSDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"


namespace Ui {
class PrincipalComponentsDialog;
}

class PrincipalComponentsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrincipalComponentsDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~PrincipalComponentsDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PrincipalComponentsDialog *ui;
    VespucciWorkspace *workspace;
    QSpinBox *component_selector_;
    QComboBox *color_selector_;
    QCheckBox *negative_score_box_;
    QLineEdit *name_box_;

    //index of the SpecMap object in the various lists
    int data_index_;

    //Pointer to the SpecMap object at data_index_
    SpecMap *data_;
};

#endif // PRINCIPALCOMPONENTSDIALOG_H
