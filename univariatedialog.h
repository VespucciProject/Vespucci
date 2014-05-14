#ifndef UNIVARIATEDIALOG_H
#define UNIVARIATEDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"

namespace Ui {
class UnivariateDialog;
}

class UnivariateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UnivariateDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~UnivariateDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::UnivariateDialog *ui;
    VespucciWorkspace *workspace;
    QCustomPlot *spectrum_plot_;
    QLineEdit *min_box_;
    QLineEdit *max_box_;
    QLineEdit *name_box_;
    QComboBox *color_selector_;
    QComboBox *value_method_selector_;
    QCheckBox *interpolation_box_;

    //index of the SpecMap object in the various lists
    int data_index_;

    //Pointer to the SpecMap object at data_index_
    SpecMap *data_;
};

#endif // UNIVARIATEDIALOG_H
