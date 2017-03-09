/*******************************************************************************
    Copyright (C) 2014-2017 Wright State University - All Rights Reserved
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
    explicit PlotWidget(QWidget *parent, QSharedPointer<VespucciWorkspace> ws);
    ~PlotWidget();
    void AddPlot(const mat & paired_data);
    void AddPlot(const vec &abscissa, const vec &data);
    void AddTransientPlot(const vec &abscissa, const vec &data);
    void AddTransientPlot(const mat & paired_data);
    void AddScatterPlot(const mat & paired_data);
    void AddScatterPlot(const vec &abscissa, const vec &data);
    void AddMappedScatterPlot(const mat & paired_data, const vec &categorical);
    void AddMappedScatterPlot(const vec & abscissa, const vec &data, const vec &categorical);
    void RemoveTransientPlot();
    bool TransientOnly() const;
    void SavePlot(QString filename);

private:
    Ui::PlotWidget *ui;
    QList<QColor> colors_;
    QColor GetNextColor();
    QSharedPointer<VespucciWorkspace> workspace_;
    QCPGraph *transient_graph_;
};

#endif // PLOTWIDGET_H
