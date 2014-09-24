#ifndef KMEANSDIALOG_H
#define KMEANSDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"

namespace Ui {
class KMeansDialog;
}

class KMeansDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KMeansDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~KMeansDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::KMeansDialog *ui;
    VespucciWorkspace *workspace;

    QSpinBox *cluster_spin_box_;
    QLineEdit *name_line_edit_;

    //index of the VespucciDataset object in the various lists
    int data_index_;

    //Pointer to the VespucciDataset object at data_index_
    QSharedPointer<VespucciDataset> data_;
};

#endif // KMEANSDIALOG_H
