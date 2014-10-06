#ifndef KMEANSDIALOG_H
#define KMEANSDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"

namespace Ui {
class KMeansDialog;
}

///
/// \brief The KMeansDialog class
/// Allows the user to create a k-means clustering map
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
    ///
    /// \brief workspace
    /// Current workspace
    VespucciWorkspace *workspace;

    ///
    /// \brief cluster_spin_box_
    /// User selects the number of clusters to compute here
    QSpinBox *cluster_spin_box_;

    ///
    /// \brief name_line_edit_
    /// User enters the name of the map here
    QLineEdit *name_line_edit_;

    ///
    /// \brief data_index_
    /// Index of the selected dataset in the master lists
    int data_index_;

    ///
    /// \brief data_
    /// The current dataset
    QSharedPointer<VespucciDataset> data_;
};

#endif // KMEANSDIALOG_H
