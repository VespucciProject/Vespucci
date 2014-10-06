#ifndef DATAEXTRACTORDIALOG_H
#define DATAEXTRACTORDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"
#include "mapdata.h"
class VespucciWorkspace;
class VespucciDataset;
class MainWindow;
class MapData;
namespace Ui {
class DataExtractorDialog;
}

///
/// \brief The DataExtractorDialog class
/// A dialog that allows the user to create a new dataset from a map, based on map values.
class DataExtractorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataExtractorDialog(QWidget *parent, MapData *map, QSharedPointer<VespucciDataset> dataset, MainWindow *main_window);
    ~DataExtractorDialog();

private slots:
    void on_buttonBox_accepted();
    
private:
    Ui::DataExtractorDialog *ui;

    ///
    /// \brief name_line_edit_
    /// User sets the dataset name here
    QLineEdit *name_line_edit_;

    ///
    /// \brief lower_box_
    /// User sets the lower bound here
    QDoubleSpinBox *lower_box_;

    ///
    /// \brief upper_box_
    /// User sets the upper bound here
    QDoubleSpinBox *upper_box_;

    ///
    /// \brief map_
    /// Pointer the map from which the dataset is formed
    MapData *map_;

    ///
    /// \brief dataset_
    /// The dataset from which the new dataset is formed
    QSharedPointer <VespucciDataset> dataset_;

    ///
    /// \brief data_range_
    /// The data range of the map (used to set limits)
    QCPRange data_range_;

    ///
    /// \brief workspace
    /// The workspace
    VespucciWorkspace *workspace;

    ///
    /// \brief main_window_
    /// The main window
    MainWindow *main_window_;
};

#endif // DATAEXTRACTORDIALOG_H
