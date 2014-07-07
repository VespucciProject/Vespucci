#ifndef SPECTRUMVIEWER_H
#define SPECTRUMVIEWER_H

#include <QDialog>
#include "mapviewer.h"
class MapViewer;
class MapData;
class SpecMap;

namespace Ui {
class SpectrumViewer;
}


class SpectrumViewer : public QDialog
{
    Q_OBJECT

public:
    explicit SpectrumViewer(MapViewer *parent,
                            MapData *map_data,
                            const QString x_axis_description,
                            const QString y_axis_description, SpecMap *dataset, QSize widget_size, QString directory);
    ~SpectrumViewer();
    void SetPlot(QVector<double> wavelength,
                 QVector<double> intensity);

    void SetSecondPlot(QVector<double> abcissa, QVector<double> intensities);
    void SetSecondPlot(QVector<double> first_abcissa, QVector<double> second_abcissa,
                       QVector<double> first_intensities, QVector<double> second_intensities);
private slots:
    void MapClicked(QCPAbstractPlottable *plottable, QMouseEvent *event);
    void on_pushButton_clicked();

private:
    Ui::SpectrumViewer *ui;
    QCustomPlot *spectrum_plot_;
    MapViewer *parent_;
    MapData *map_data_;
    SpecMap *dataset_;
    QSize widget_size_;
    QString directory_;
    int current_index_;
    double current_x_;
    double current_y_;
    QLabel *coordinate_label_;
    QLabel *value_label_;
};

#endif // SPECTRUMVIEWER_H
