#ifndef HASPEAKSDIALOG_H
#define HASPEAKSDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class HasPeaksDialog;
}

class HasPeaksDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HasPeaksDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~HasPeaksDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::HasPeaksDialog *ui;
    QSharedPointer<VespucciDataset> dataset_;
    QDoubleSpinBox *min_box_;
    QDoubleSpinBox *max_box_;
    QTableWidget *table_widget_;
    VespucciWorkspace *workspace;

};

#endif // HASPEAKSDIALOG_H
