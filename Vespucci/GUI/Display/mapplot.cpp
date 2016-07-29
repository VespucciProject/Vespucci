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
#include "mapplot.h"

MapPlot::MapPlot(QWidget *parent)
    : QCustomPlot(parent)
{
    vertical_crosshair_ = new QCPItemStraightLine(this);
    horizontal_crosshair_ = new QCPItemStraightLine(this);
    color_map_ = new QCPColorMap(xAxis, yAxis);
    color_scale_ = new QCPColorScale(this);
    color_map_->setColorScale(color_scale_);
    color_map_->setInterpolate(false);
    addItem(vertical_crosshair_);
    addItem(horizontal_crosshair_);
    addPlottable(color_map_);
    plotLayout()->addElement(0, 1, color_scale_);
    CenterCrosshairs();
}

///
/// \brief MapPlot::SetMapData
/// \param x
/// \param y
/// \param z
/// Throws an exception when vectors do not have equal sizes
void MapPlot::SetMapData(const vec &x, const vec &y, const vec &z)
{
    if ((x.n_elem != y.n_elem) || (x.n_elem != z.n_elem) || (y.n_elem != z.n_elem))
        throw std::invalid_argument("Vector sizes do not match!");
    y_ = y;
    x_ = x;
    z_ = z;
    QCPRange x_range(x_.min(), x_.max());
    QCPRange y_range(y_.min(), y_.max());
    uword x_size = vec(unique(x_)).n_rows;
    uword y_size = vec(unique(y_)).n_rows;


    color_map_->data()->setKeySize(x_size);
    color_map_->data()->setValueSize(y_size);
    color_map_->data()->setKeyRange(x_range);
    color_map_->data()->setValueRange(y_range);
    for (uword i = 0; i < x.n_elem; ++i){
        color_map_->data()->setData(x_(i), y_(i), z_(i));
    }
    color_map_->rescaleDataRange(true);
    rescaleAxes();
    CenterCrosshairs();
    replot();
}

void MapPlot::SetColorScale(QCPColorScale *scale)
{
    color_scale_ = scale;
    color_map_->setColorScale(scale);
    replot();
}

void MapPlot::SetGradient(QCPColorGradient gradient)
{
    color_map_->setGradient(gradient);
    color_map_->rescaleDataRange(true);
    replot();
}

void MapPlot::SetGlobalColorGradient(Vespucci::GlobalGradient gradient)
{
    color_scale_->setDataRange(gradient.range);
    color_scale_->setGradient(gradient.gradient);
    color_map_->setGradient(gradient.gradient);
}

///
/// \brief MapPlot::CenterCrosshairs
/// Positions the crosshairs in the center of the map
void MapPlot::CenterCrosshairs()
{
    double x_min = x_.min();
    double x_max = x_.max();
    double y_min = y_.min();
    double y_max = y_.max();
    double x_center = (x_min + x_max) / 2.0;
    double y_center = (y_min + y_max) / 2.0;


    horizontal_crosshair_->point1->setCoords(x_min, y_center);
    horizontal_crosshair_->point2->setCoords(x_max, y_center);
    vertical_crosshair_->point1->setCoords(x_center, y_min);
    vertical_crosshair_->point2->setCoords(x_center, y_max);

    horizontal_crosshair_->setVisible(true);
    vertical_crosshair_->setVisible(true);

}

///
/// \brief MapPlot::GetCrossHairPosition
/// \param x
/// \param y
/// \param z
/// \return
/// Get the index of the spectrum the crosshairs are centered on
uword MapPlot::GetCrosshairPosition(double &x, double &y, double &z) const
{
    uword index = GetCrosshairPosition();
    x = x_(index);
    y = y_(index);
    z = z_(index);
    return index;
}

uword MapPlot::GetCrosshairPosition() const
{
    double x_pos = vertical_crosshair_->point1->coords().x();
    double y_pos = horizontal_crosshair_->point1->coords().y();

    //find the values of x and y closest to the position of the crosshairs
    vec y_diff = y_ - y_pos*ones(y_.n_rows);
    vec x_diff = x_ - x_pos*ones(x_.n_rows);
    vec combined = y_diff + x_diff;

    return combined.index_min();
}

double MapPlot::min() const
{
    return z_.min();
}

double MapPlot::max() const
{
    return z_.max();
}

///
/// \brief MapPlot::MoveVerticalCrosshair
/// \param units
/// Moves the vertical crosshair left or right by a multiple of the x step
void MapPlot::MoveVerticalCrosshair(int units)
{
    double x_step = x_(1) - x_(0);
    QPointF point1_pos = vertical_crosshair_->point1->coords();
    QPointF point2_pos = vertical_crosshair_->point2->coords();

    point1_pos.setX(point1_pos.x() + (double(units) * x_step));
    point2_pos.setX(point2_pos.x() + (double(units) * x_step));

    vertical_crosshair_->point1->setCoords(point1_pos);
    vertical_crosshair_->point2->setCoords(point2_pos);
    replot(QCustomPlot::rpImmediate);
    emit SpectrumRequested(GetCrosshairPosition());
}

///
/// \brief MapPlot::MoveHorizontalCrosshair
/// \param units
/// moves the horizontal crosshair up or down by a multiple of the y step
void MapPlot::MoveHorizontalCrosshair(int units)
{
    double y_step = y_(1) - y_(0);
    QPointF point1_pos = horizontal_crosshair_->point1->coords();
    QPointF point2_pos = horizontal_crosshair_->point2->coords();

    point1_pos.setY(point1_pos.y() + (double(units) * y_step));
    point2_pos.setY(point2_pos.y() + (double(units) * y_step));

    horizontal_crosshair_->point1->setCoords(point1_pos);
    horizontal_crosshair_->point2->setCoords(point2_pos);

    replot(QCustomPlot::rpImmediate);
    emit SpectrumRequested(GetCrosshairPosition());
}

void MapPlot::rescaleDataRange(bool onlyVisibleMaps)
{
    color_scale_->rescaleDataRange(onlyVisibleMaps);
}

void MapPlot::setDataRange(const QCPRange &dataRange)
{
    color_scale_->setDataRange(dataRange);
    replot();
    repaint();
}

void MapPlot::ColorScaleVisible(bool visible)
{
    color_scale_->setVisible(visible);
}

void MapPlot::CrossHairsVisible(bool visible)
{
    vertical_crosshair_->setVisible(visible);
    horizontal_crosshair_->setVisible(visible);
}

void MapPlot::SetFonts(const QFont &font)
{
    setFont(font);

    xAxis->setLabelFont(font);
    xAxis->setTickLabelFont(font);

    xAxis2->setLabelFont(font);
    xAxis2->setTickLabelFont(font);

    yAxis->setLabelFont(font);
    yAxis->setTickLabelFont(font);

    yAxis2->setLabelFont(font);
    yAxis2->setTickLabelFont(font);

    legend->setFont(font);

    color_scale_->axis()->setLabelFont(font);
    color_scale_->axis()->setTickLabelFont(font);
}

void MapPlot::SetColorScaleLabel(const QString &label_text)
{
    color_scale_->setLabel(label_text);
}

QString MapPlot::ColorScaleLabel() const
{
    return color_scale_->label();
}

void MapPlot::SetColorScaleTickCount(int ticks)
{
    color_scale_->axis()->setAutoTickCount(ticks);
}

void MapPlot::SetColorScaleTicks(double min, double max, size_t count)
{
    color_scale_->axis()->setAutoTicks(false);
    vec ticks = linspace(min, max, count);
    qvec tick_qv = qvec::fromStdVector(conv_to<stdvec>::from(ticks));
    color_scale_->axis()->setAutoTickLabels(true);
    color_scale_->axis()->setTickVector(tick_qv);
    replot(QCustomPlot::rpImmediate);
}

void MapPlot::SetClusterTicks(size_t count)
{
    vec ticks = linspace(1, count, count);
    //shift ticks up to match center of color stops
    QVector<QString> tick_labels;
}

void MapPlot::setInterpolate(bool interpolate)
{
    color_map_->setInterpolate(interpolate);
}

bool MapPlot::interpolate() const
{
    return color_map_->interpolate();
}

int MapPlot::keySize()
{
    return color_map_->data()->keySize();
}

int MapPlot::valueSize()
{
    return color_map_->data()->valueSize();
}

QCPRange MapPlot::keyRange()
{
    return color_map_->keyAxis()->range();
}

QCPRange MapPlot::valueRange()
{
    return color_map_->valueAxis()->range();
}

void MapPlot::ShowCrosshairs(bool show)
{
    vertical_crosshair_->setVisible(show);
    horizontal_crosshair_->setVisible(show);
}
