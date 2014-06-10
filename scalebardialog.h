#ifndef SCALEBARDIALOG_H
#define SCALEBARDIALOG_H

#include <QDialog>
#include "mapviewer.h"

class MapData;
class MapViewer;

namespace Ui {
class ScaleBarDialog;
}

class ScaleBarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScaleBarDialog(QWidget *parent, MapData *map_data);
    ~ScaleBarDialog();

private slots:
    void on_colorPushButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::ScaleBarDialog *ui;
    QDoubleSpinBox *width_box_;
    QDoubleSpinBox *height_box_;
    QLineEdit *units_box_;
    QPushButton *color_button_;
    QComboBox *position_box_;
    QColor color_;
    MapData *map_data_;
};

#endif // SCALEBARDIALOG_H
