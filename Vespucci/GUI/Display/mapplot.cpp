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
    QPen crosshair_pen(Qt::red);
    crosshair_pen.setWidth(4);
    vertical_crosshair_->setPen(crosshair_pen);
    horizontal_crosshair_->setPen(crosshair_pen);
    color_map_ = new QCPColorMap(xAxis, yAxis);
    color_scale_ = new QCPColorScale(this);
    color_map_->setColorScale(color_scale_);
    color_map_->setInterpolate(false);
    color_map_->setTightBoundary(false);
    addLayer("crosshairs");
    vertical_crosshair_->setLayer("crosshairs");
    horizontal_crosshair_->setLayer("crosshairs");
    addItem(vertical_crosshair_);
    addItem(horizontal_crosshair_);
    addPlottable(color_map_);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    plotLayout()->addElement(0, 1, color_scale_);
    addLayer("colorscale");
    color_scale_->setLayer("colorscale");

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
    vec unique_x = sort(unique(x_));
    vec unique_y = sort(unique(y_));
    uword x_size = unique_x.n_rows;
    uword y_size = unique_y.n_rows;
    x_step_ = unique_x(1) - unique_x(0);
    y_step_ = unique_y(1) - unique_y(0);

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
    replot(QCustomPlot::rpImmediate);
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
uword MapPlot::GetCrosshairPosition(double &x, double &y, double &z)
{
    uword index = GetCrosshairPosition();
    x = x_(index);
    y = y_(index);
    z = z_(index);
    return index;
}

uword MapPlot::GetCrosshairPosition()
{
    double x_pos = vertical_crosshair_->point1->coords().x();
    double y_pos = horizontal_crosshair_->point1->coords().y();
    double z_val = color_map_->data()->cell(x_pos, y_pos);
    emit CoordinatesChanged(x_pos, y_pos, z_val);

    if (x_pos > x_.max()) x_pos = x_.max();
    if (x_pos < x_.min()) x_pos = x_.min();
    if (y_pos > y_.max()) y_pos = y_.max();
    if (y_pos < y_.min()) y_pos = y_.min();

    //find the values of x and y closest to the position of the crosshairs
    uvec x_ind = arma::find(x_ < x_pos + 0.01 && x_ > x_pos - 0.01);
    uvec y_ind = arma::find(y_ < y_pos + 0.01 && y_ > y_pos - 0.01);
    uvec intersection = Vespucci::Math::Intersection(x_ind, y_ind);
    uword index = intersection(0);
    emit CoordinatesChanged(x_(index), y_(index), z_(index));
    return index;
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
    QPointF point1_pos = vertical_crosshair_->point1->coords();
    QPointF point2_pos = vertical_crosshair_->point2->coords();

    double new_x = point1_pos.x() + double(units)*x_step_;
    new_x = (new_x > x_.max() ? x_.max() : new_x);
    new_x = (new_x < x_.min() ? x_.min() : new_x);

    point1_pos.setX(new_x);
    point2_pos.setX(new_x);

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
    QPointF point1_pos = horizontal_crosshair_->point1->coords();
    QPointF point2_pos = horizontal_crosshair_->point2->coords();
    double new_y = point1_pos.y() + double(units)*y_step_;
    new_y = (new_y > y_.max() ? y_.max() : new_y);
    new_y = (new_y < y_.min() ? y_.min() : new_y);

    point1_pos.setY(new_y);
    point2_pos.setY(new_y);

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
