#ifndef DATAEXTRACTORDIALOG_H
#define DATAEXTRACTORDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"
#include "mapdata.h"
class VespucciWorkspace;
class SpecMap;
class MainWindow;
class MapData;
namespace Ui {
class DataExtractorDialog;
}

class DataExtractorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataExtractorDialog(QWidget *parent, MapData *map, SpecMap *dataset, MainWindow *main_window);
    ~DataExtractorDialog();

private slots:
    void on_buttonBox_accepted();
    
private:
    Ui::DataExtractorDialog *ui;
    QLineEdit *name_line_edit_;
    QDoubleSpinBox *lower_box_;
    QDoubleSpinBox *upper_box_;
    MapData *map_;
    SpecMap *dataset_;
    QCPRange data_range_;
    VespucciWorkspace *workspace;
    MainWindow *main_window_;
};

#endif // DATAEXTRACTORDIALOG_H
