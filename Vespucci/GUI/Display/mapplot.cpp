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
    vertical_crosshair_ = new QCPItemLine(this);
    horizontal_crosshair_ = new QCPItemLine(this);
    color_map_ = new QCPColorMap(xAxis, yAxis);
    color_scale_ = new QCPColorScale(this);
    color_map_->setColorScale(color_scale_);
    addPlottable(color_map_);
    addItem(vertical_crosshair_);
    addItem(horizontal_crosshair_);
    plotLayout()->addElement(0, 1, color_scale_);
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
    //in case the instrument rounds differently.
    y_ = linspace(y.min(), y.max(), y.n_rows);
    x_ = linspace(x.min(), x.max(), x.n_rows);
    z_ = z;

    for (uword i = 0; i < x.n_elem; ++i){
        color_map_->data()->setData(x_(i), y_(i), z(i));
    }
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
    double x_min = color_map_->data()->keyRange().lower;
    double x_max = color_map_->data()->keyRange().upper;
    double y_min = color_map_->data()->valueRange().lower;
    double y_max = color_map_->data()->valueRange().upper;
    double x_center = xAxis->range().center();
    double y_center = yAxis->range().center();

    horizontal_crosshair_->start->setCoords(x_min, y_center);
    horizontal_crosshair_->end->setCoords(x_max, y_center);
    vertical_crosshair_->start->setCoords(x_center, y_min);
    vertical_crosshair_->end->setCoords(x_center, y_max);
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
    double x_pos = vertical_crosshair_->start->coords().x();
    double y_pos = horizontal_crosshair_->start->coords().y();

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
/// \brief MapPlot::keyPressEvent
/// \param event
/// Override of QWidget::keyPressEvent to handle arrow keys and enter key
void MapPlot::keyPressEvent(QKeyEvent *event)
{
    //arrow keys on some laptops might be difficult to use
    //so why not also accept WASD and HJKL?

    switch (event->key()) {
        case Qt::Key_Up:
        case Qt::Key_W:
        case Qt::Key_K:
            MoveHorizontalCrosshair(1);
            return;
        case Qt::Key_Down:
        case Qt::Key_S:
        case Qt::Key_J:
            MoveHorizontalCrosshair(-1);
            return;
        case Qt::Key_Left:
        case Qt::Key_A:
        case Qt::Key_H:
            MoveVerticalCrosshair(-1);
            return;
        case Qt::Key_Right:
        case Qt::Key_F:
        case Qt::Key_L:
            MoveVerticalCrosshair(1);
            return;
        case Qt::Key_Enter:
        case Qt::Key_Space:
            emit SpectrumHoldRequested(GetCrosshairPosition());
            return;
        default:
            return;
    }
}

///
/// \brief MapPlot::MoveVerticalCrosshair
/// \param units
/// Moves the vertical crosshair left or right by a multiple of the x step
void MapPlot::MoveVerticalCrosshair(int units)
{
    double x_step = (color_map_->data()->keyRange().upper
                     - color_map_->data()->keyRange().lower)
            / double(color_map_->data()->keySize());

    double current_x = vertical_crosshair_->start->coords().x();
    double start_y = vertical_crosshair_->start->coords().y();
    double end_y = vertical_crosshair_->end->coords().y();
    QCPRange x_range = color_map_->data()->keyRange();
    double new_x;

    if (x_range.contains(current_x + x_step * double(units)))
        new_x = current_x + x_step * double(units);
    else if (current_x + x_step * double(units) >= x_range.upper)
        new_x = x_range.upper;
    else
        new_x = x_range.lower;
    vertical_crosshair_->start->setCoords(new_x, start_y);
    vertical_crosshair_->end->setCoords(new_x, end_y);
    repaint();
    emit SpectrumRequested(GetCrosshairPosition());
}

///
/// \brief MapPlot::MoveHorizontalCrosshair
/// \param units
/// moves the horizontal crosshair up or down by a multiple of the y step
void MapPlot::MoveHorizontalCrosshair(int units)
{
    double y_step = (color_map_->data()->valueRange().upper
                     - color_map_->data()->valueRange().lower)
            / double(color_map_->data()->valueSize());

    double current_y = horizontal_crosshair_->start->coords().y();
    double start_x = horizontal_crosshair_->start->coords().x();
    double end_x = horizontal_crosshair_->end->coords().x();
    QCPRange y_range = color_map_->data()->keyRange();
    double new_y;

    if (y_range.contains(current_y + y_step * double(units)))
        new_y = current_y + y_step * double(units);
    else if (current_y + y_step * double(units) >= y_range.upper)
        new_y = y_range.upper;
    else
        new_y = y_range.lower;
    vertical_crosshair_->start->setCoords(start_x, new_y);
    vertical_crosshair_->end->setCoords(end_x, new_y);
    repaint();
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
