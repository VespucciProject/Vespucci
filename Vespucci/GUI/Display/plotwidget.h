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
#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <qcustomplot.h>
#include <QVector>
#include <mlpack/core.hpp>
#include <Global/vespucciworkspace.h>
class VespucciWorkspace;
namespace Ui {
class PlotWidget;
}
using namespace std;
using namespace arma;
typedef vector<double> stdvec;
class PlotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlotWidget(QWidget *parent = 0, VespucciWorkspace *ws=0);
    ~PlotWidget();
    void AddPlot(const mat & paired_data);
    void AddPlot(const vec &abscissa, const vec &data);
    void AddTransientPlot(const vec &abscissa, const vec &data);
    void AddTransientPlot(const mat & paired_data);
    void RemoveTransientPlot();
    void StackPlots(bool stack);
    bool offset_plots() const;
    bool TransientOnly() const;

public slots:
private:
    Ui::PlotWidget *ui;
    QCustomPlot *plot_;
    double offset_by_;
    bool offset_plots_;
    void DetermineOffset();
    void DetermineOffset(const QVector<double> &new_data);
    void ApplyOffset();
    void RemoveOffset();
    QVector<double> GetData(const QList<QCPData> &values) const;
    QVector<double> GetAbscissa(const QList<QCPData> &keys) const;
    QList<QColor> colors_;
    QColor GetNextColor();
    VespucciWorkspace *workspace;
    QCPGraph *transient_graph_;
};

#endif // PLOTWIDGET_H
