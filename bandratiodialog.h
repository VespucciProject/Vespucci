#ifndef BANDRATIODIALOG_H
#define BANDRATIODIALOG_H

#include <QDialog>
#include "univariatedialog.h"

namespace Ui {
class BandRatioDialog;
}

class BandRatioDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BandRatioDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~BandRatioDialog();

private:
    Ui::BandRatioDialog *ui;

    VespucciWorkspace *workspace;
    QCustomPlot *spectrum_plot_;
    QLineEdit *first_min_box_;
    QLineEdit *first_max_box_;
    QLineEdit *second_min_box_;
    QLineEdit *second_max_box_;

    QLineEdit *name_box_;
    QComboBox *color_selector_;
    QComboBox *value_method_selector_;

    //index of the SpecMap object in the various lists
    int data_index_;

    //Pointer to the SpecMap object at data_index_
    SpecMap *data_;
};

#endif // BANDRATIODIALOG_H
