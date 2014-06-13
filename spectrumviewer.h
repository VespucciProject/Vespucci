#ifndef SPECTRUMVIEWER_H
#define SPECTRUMVIEWER_H

#include <QDialog>
#include "mapdata.h"

namespace Ui {
class SpectrumViewer;
}

class SpectrumViewer : public QDialog
{
    Q_OBJECT

public:
    explicit SpectrumViewer(QWidget parent = 0, MapData *map_data = 0);
    ~SpectrumViewer();
    void SetPlot(QVector wavelength,
                 QVector intensity,
                 QString x_axis_description,
                 QString y_axis_description);

private:
    Ui::SpectrumViewer *ui;
    QCustomPlot *spectrum_plot_;
};

#endif // SPECTRUMVIEWER_H
