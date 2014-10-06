#ifndef PLSDIALOG_H
#define PLSDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"

namespace Ui {
class PLSDialog;
}

///
/// \brief The PLSDialog class
/// Dialog that allows the user to perform PLS determinant analysis
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
    ///
    /// \brief workspace
    /// Current workspace
    VespucciWorkspace *workspace;

    ///
    /// \brief image_component_selector_
    /// Which component number to create the image from
    QSpinBox *image_component_selector_;

    ///
    /// \brief components_selector_
    /// User selects how many components to calculate
    QSpinBox *components_selector_;

    ///
    /// \brief color_selector_
    /// User selects color scheme here
    QComboBox *color_selector_;

    ///
    /// \brief name_box_
    /// User sets map name here
    QLineEdit *name_box_;

    ///
    /// \brief recalculate_box_
    /// Whether or not to repeat PLS analysis
    QCheckBox *recalculate_box_;

    ///
    /// \brief data_index_
    /// Index of the dataset in the various lists
    int data_index_;

    ///
    /// \brief data_
    /// Current dataset
    QSharedPointer<VespucciDataset> data_;
};

#endif // PLSDIALOG_H
