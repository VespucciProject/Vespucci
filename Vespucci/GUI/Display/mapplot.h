/*******************************************************************************
    Copyright (C) 2014-2016 Wright State University - All Rights Reserved
    Daniel P. Foose - Maintainer/Lead Developer

    This file is part of Vespucci.

    Vespucci is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Vespucci is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Vespucci.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
#ifndef MAPPLOT_H
#define MAPPLOT_H

#include "qcustomplot.h"
#include "Global/vespucciworkspace.h"
#include "Global/global.h"
using namespace arma;
typedef std::vector<double> stdvec;
typedef QVector<double> qvec;

///
/// \brief The MapPlot class
/// A subclass of QCustomPlot for handling a specfic kind of color map.
/// Provides a wrapper for a QCPColorScale and a QCPColorMap which are child widgets.
class MapPlot : public QCustomPlot
{
    Q_OBJECT

public:
    MapPlot(QWidget *parent = 0);
    void SetMapData(const vec &x, const vec &y, const vec &z);
    void SetColorScale(QCPColorScale *scale);
    void SetGradient(QCPColorGradient gradient);
    void SetGlobalColorGradient(Vespucci::GlobalGradient gradient);
    void CenterCrosshairs();
    uword GetCrosshairPosition(double &x, double &y, double &z) const;
    uword GetCrosshairPosition() const;
    double min() const;
    double max() const;
    void rescaleDataRange(bool onlyVisibleMaps = false);
    void setDataRange(const QCPRange &dataRange);
    void ColorScaleVisible(bool visible);
    void CrossHairsVisible(bool visible);
    void SetFonts(const QFont &font);
    void SetColorScaleLabel(const QString &label_text);
    QString ColorScaleLabel() const;
    void SetColorScaleTickCount(int ticks);
    void SetColorScaleTicks(double min, double max, size_t count);
    void SetClusterTicks(size_t count);
    void setInterpolate(bool interpolate);
    bool interpolate() const;
    int keySize();
    int valueSize();
    QCPRange keyRange();
    QCPRange valueRange();
    void ShowCrosshairs(bool show);
    void MoveVerticalCrosshair(int units);
    void MoveHorizontalCrosshair(int units);
signals:
    void SpectrumRequested(size_t index);
    void SpectrumHoldRequested(size_t index);
private:
    QCPColorMap *color_map_;
    QCPItemStraightLine *horizontal_crosshair_;
    QCPItemStraightLine *vertical_crosshair_;
    QCPColorScale *color_scale_;
    vec x_;
    vec y_;
    vec z_;
};

#endif // MAPPLOT_H
