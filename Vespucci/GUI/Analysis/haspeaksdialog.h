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
    explicit HasPeaksDialog(QWidget *parent, VespucciWorkspace *ws, const QModelIndex &dataset_index);
    ~HasPeaksDialog();

private slots:
    void on_buttonBox_accepted();

    void on_addPushButton_clicked();

    void on_removePushButton_2_clicked();

private:
    Ui::HasPeaksDialog *ui;
    QSharedPointer<VespucciDataset> dataset_;
    QTableWidget *table_widget_;
    VespucciWorkspace *workspace;

};

#endif // HASPEAKSDIALOG_H
