#ifndef SPECTRUMVIEWER_H
#define SPECTRUMVIEWER_H

#include <QDialog>
#include "mapviewer.h"
class MapViewer;
class MapData;

namespace Ui {
class SpectrumViewer;
}


class SpectrumViewer : public QDialog
{
    Q_OBJECT

public:
    explicit SpectrumViewer(MapViewer *parent,
                            MapData *map_data,
                            QVector<double> wavelength,
                            QVector<double> initial_data,
                            const QString x_axis_description,
                            const QString y_axis_description);
    ~SpectrumViewer();
    void SetPlot(QVector<double> wavelength,
                 QVector<double> intensity);

private:
    Ui::SpectrumViewer *ui;
    QCustomPlot *spectrum_plot_;
    MapViewer *parent_;
    MapData *map_data_;
};

#endif // SPECTRUMVIEWER_H
