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
#include "plotwidget.h"
#include "ui_plotwidget.h"
#include "Global/global.h"

PlotWidget::PlotWidget(QWidget *parent, QSharedPointer<VespucciWorkspace> ws) :
    QWidget(parent),
    ui(new Ui::PlotWidget)
{
    workspace_ = ws;
    transient_graph_ = 0;
    ui->setupUi(this);
    ui->customPlot->setInteraction(QCP::iRangeDrag);
    ui->customPlot->setInteraction(QCP::iRangeZoom);
    ui->customPlot->setInteraction(QCP::iSelectItems);
    ui->customPlot->setInteraction(QCP::iMultiSelect);
    ui->customPlot->setInteraction(QCP::iSelectLegend);
    ui->customPlot->setInteraction(QCP::iSelectOther);
    ui->customPlot->setInteraction(QCP::iSelectPlottables);

    colors_ = {QColor(228,26,28),
               QColor(55,126,184),
               QColor(77,175,74),
               QColor(152,78,163),
               QColor(255,127,0),
               QColor(166,86,40),
               QColor(247,129,191),
               QColor(153,153,153)};
}

PlotWidget::~PlotWidget()
{
    delete ui;
}

void PlotWidget::AddPlot(const mat & paired_data)
{
    QColor pen_color = this->GetNextColor();
    QPen pen(pen_color);
    if (paired_data.n_cols < 2)
        return;
    qvec abscissa =
            qvec::fromStdVector(conv_to<stdvec>::from(paired_data.col(0)));
    qvec data =
            qvec::fromStdVector(conv_to<stdvec>::from(paired_data.col(1)));
    int graph_count = ui->customPlot->graphCount();
    if (graph_count < 1){
        ui->customPlot->addGraph();
        ui->customPlot->graph(graph_count)->addData(abscissa, data);
    }
    else{
        ui->customPlot->addGraph(ui->customPlot->graph(0)->keyAxis(), ui->customPlot->graph(0)->valueAxis());
        ui->customPlot->graph(graph_count)->addData(abscissa, data);
    }
    ui->customPlot->graph(graph_count)->setPen(pen);
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot(QCustomPlot::rpImmediate);
}

void PlotWidget::AddPlot(const vec &abscissa, const vec &data)
{
    QColor pen_color = this->GetNextColor();
    QPen pen(pen_color);
    qvec abscissa_qvec =
            qvec::fromStdVector(conv_to<stdvec>::from(abscissa));
    qvec data_qvec =
            qvec::fromStdVector(conv_to<stdvec>::from(data));
    int graph_count = ui->customPlot->graphCount();
    if (graph_count < 1){
        ui->customPlot->addGraph();
        ui->customPlot->graph(graph_count)->addData(abscissa_qvec, data_qvec);
    }
    else{
        ui->customPlot->addGraph(ui->customPlot->graph(0)->keyAxis(), ui->customPlot->graph(0)->valueAxis());
        ui->customPlot->graph(graph_count)->addData(abscissa_qvec, data_qvec);
    }
    ui->customPlot->graph(graph_count)->setPen(pen);
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot(QCustomPlot::rpImmediate);
}

void PlotWidget::AddTransientPlot(const vec &abscissa, const vec &data)
{
    QColor pen_color = this->GetNextColor();
    QPen pen(pen_color);
    if (abscissa.n_rows != data.n_rows) return;

    if (transient_graph_){
        qvec abscissa_qvec =
                qvec::fromStdVector(conv_to<vector<double> >::from(abscissa));
        qvec data_qvec =
                qvec::fromStdVector(conv_to<vector<double> >::from(data));
        transient_graph_->setData(abscissa_qvec, data_qvec);
    }
    else{
        QCPAxis *key_axis;
        QCPAxis *value_axis;
        if (ui->customPlot->graphCount()){
            key_axis = ui->customPlot->graph(0)->keyAxis();
            value_axis = ui->customPlot->graph(0)->valueAxis();
        }
        else{
            key_axis = 0;
            value_axis = 0;
        }
        ui->customPlot->addGraph(key_axis, value_axis);
        transient_graph_ = ui->customPlot->graph(ui->customPlot->graphCount() - 1);
        qvec abscissa_qvec =
                qvec::fromStdVector(conv_to<vector<double> >::from(abscissa));
        qvec data_qvec =
                qvec::fromStdVector(conv_to<vector<double> >::from(data));
        transient_graph_->addData(abscissa_qvec, data_qvec);
    }
    transient_graph_->setPen(pen);
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot(QCustomPlot::rpImmediate);
}

void PlotWidget::AddTransientPlot(const mat & paired_data)
{
    QColor pen_color = this->GetNextColor();
    QPen pen(pen_color);

    if (paired_data.n_cols != 2) return;

    QCPAxis *key_axis;
    QCPAxis *value_axis;
    if (transient_graph_){
        qvec abscissa_qvec =
                qvec::fromStdVector(conv_to<vector<double> >::from(paired_data.col(0)));
        qvec data_qvec =
                qvec::fromStdVector(conv_to<vector<double> >::from(paired_data.col(1)));
        transient_graph_->setData(abscissa_qvec, data_qvec);
    }
    else{
        if (ui->customPlot->graphCount()){
            key_axis = ui->customPlot->graph(0)->keyAxis();
            value_axis = ui->customPlot->graph(0)->valueAxis();
        }
        else{
            key_axis = 0;
            value_axis = 0;
        }
        ui->customPlot->addGraph(key_axis, value_axis);
        transient_graph_ = ui->customPlot->graph(ui->customPlot->graphCount() - 1);
        qvec abscissa_qvec =
                qvec::fromStdVector(conv_to<vector<double> >::from(paired_data.col(0)));
        qvec data_qvec =
                qvec::fromStdVector(conv_to<vector<double> >::from(paired_data.col(1)));
        transient_graph_->addData(abscissa_qvec, data_qvec);
    }
    transient_graph_->setPen(pen);
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot(QCustomPlot::rpImmediate);
}

///
/// \brief PlotWidget::AddScatterPlot
/// \param paired_data
///
void PlotWidget::AddScatterPlot(const mat &paired_data)
{
    QColor pen_color = this->GetNextColor();
    QPen pen(pen_color);
    if (paired_data.n_cols < 2)
        return;
    qvec abscissa =
            qvec::fromStdVector(conv_to<stdvec>::from(paired_data.col(0)));
    qvec data =
            qvec::fromStdVector(conv_to<stdvec>::from(paired_data.col(1)));
    int graph_count = ui->customPlot->graphCount();
    if (graph_count < 1){
        ui->customPlot->addGraph();
        ui->customPlot->graph(graph_count)->addData(abscissa, data);
    }
    else{
        ui->customPlot->addGraph(ui->customPlot->graph(0)->keyAxis(), ui->customPlot->graph(0)->valueAxis());
        ui->customPlot->graph(graph_count)->addData(abscissa, data);
    }
    ui->customPlot->graph(graph_count)->setPen(pen);

    QCPScatterStyle style(QCPScatterStyle::ssDisc, pen_color, 4);
    ui->customPlot->graph(graph_count)->setScatterStyle(style);
    ui->customPlot->graph(graph_count)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot(QCustomPlot::rpImmediate);
}

///
/// \brief PlotWidget::AddScatterPlot
/// \param abscissa
/// \param data
///
void PlotWidget::AddScatterPlot(const vec &abscissa, const vec &data)
{
    QColor pen_color = this->GetNextColor();
    qvec abscissa_qvec =
            qvec::fromStdVector(conv_to<stdvec>::from(abscissa));
    qvec data_qvec =
            qvec::fromStdVector(conv_to<stdvec>::from(data));
    int graph_count = ui->customPlot->graphCount();
    if (graph_count < 1){
        ui->customPlot->addGraph();
        ui->customPlot->graph(graph_count)->addData(abscissa_qvec, data_qvec);
    }
    else{
        ui->customPlot->addGraph(ui->customPlot->graph(0)->keyAxis(), ui->customPlot->graph(0)->valueAxis());
        ui->customPlot->graph(graph_count)->addData(abscissa_qvec, data_qvec);
    }

    QCPScatterStyle style(QCPScatterStyle::ssDisc, pen_color, 4);
    ui->customPlot->graph(graph_count)->setScatterStyle(style);
    ui->customPlot->graph(graph_count)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot(QCustomPlot::rpImmediate);
}

///
/// \brief PlotWidget::AddMappedScatterPlot
/// \param paired_data
/// \param categorical
///
void PlotWidget::AddMappedScatterPlot(const mat &paired_data, const vec &categorical)
{
    //ignore incomplete categorical data
    if (categorical.n_rows != paired_data.n_rows){
        AddScatterPlot(paired_data);
        return;
    }

    vec unique_values = unique(categorical);
    int color_index;
    for (uword i = 0; i < unique_values.n_rows; ++i){
        color_index = i;
        while (color_index >= colors_.size()){color_index -= colors_.size();}
        QColor color = colors_[color_index];
        QCPScatterStyle style(QCPScatterStyle::ssDisc, color, 4);
        double value = unique_values(i);
        uvec indices = arma::find(categorical == value);
        mat current_data = paired_data.rows(indices);
        qvec x = qvec::fromStdVector(conv_to<stdvec>::from(current_data.col(0)));
        qvec y = qvec::fromStdVector(conv_to<stdvec>::from(current_data.col(1)));

        int graph_count = ui->customPlot->graphCount();
        if (graph_count < 1){
            ui->customPlot->addGraph();
            ui->customPlot->graph(graph_count)->addData(x, y);
        }
        else{
            ui->customPlot->addGraph(ui->customPlot->graph(0)->keyAxis(), ui->customPlot->graph(0)->valueAxis());
            ui->customPlot->graph(graph_count)->addData(x, y);
        }

        ui->customPlot->graph(graph_count)->setScatterStyle(style);
        ui->customPlot->graph(graph_count)->setLineStyle(QCPGraph::lsNone);
    }
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot(QCustomPlot::rpImmediate);
}

///
/// \brief PlotWidget::AddMappedScatterPlot
/// \param abscissa
/// \param data
/// \param categorical
///
void PlotWidget::AddMappedScatterPlot(const vec &abscissa, const vec &data, const vec &categorical)
{
    if (data.n_rows != abscissa.n_rows) return;
    //ignore incomplete categorical data
    if (categorical.n_rows != data.n_rows){
        AddScatterPlot(abscissa, data);
        return;
    }

    vec unique_values = unique(categorical);
    int color_index;
    for (uword i = 0; i < unique_values.n_rows; ++i){
        color_index = i;
        while (color_index >= colors_.size()){color_index -= colors_.size();}
        QColor color = colors_[color_index];
        QCPScatterStyle style(QCPScatterStyle::ssDisc, color, 4);
        double value = unique_values(i);
        uvec indices = arma::find(categorical == value);
        vec current_data = data.rows(indices);
        vec current_abscissa = abscissa.rows(indices);
        qvec x = qvec::fromStdVector(conv_to<stdvec>::from(current_abscissa));
        qvec y = qvec::fromStdVector(conv_to<stdvec>::from(current_data));

        int graph_count = ui->customPlot->graphCount();
        if (graph_count < 1){
            ui->customPlot->addGraph();
            ui->customPlot->graph(graph_count)->addData(x, y);
        }
        else{
            ui->customPlot->addGraph(ui->customPlot->graph(0)->keyAxis(), ui->customPlot->graph(0)->valueAxis());
            ui->customPlot->graph(graph_count)->addData(x, y);
        }

        ui->customPlot->graph(graph_count)->setScatterStyle(style);
        ui->customPlot->graph(graph_count)->setLineStyle(QCPGraph::lsNone);

    }
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot(QCustomPlot::rpImmediate);
}

void PlotWidget::RemoveTransientPlot()
{
    if (transient_graph_){
        ui->customPlot->removeGraph(transient_graph_);
        transient_graph_ = 0;
        ui->customPlot->rescaleAxes();
        ui->customPlot->replot(QCustomPlot::rpImmediate);
    }
}

bool PlotWidget::TransientOnly() const
{
    return (transient_graph_ && ui->customPlot->graphCount() == 1);
}

void PlotWidget::SavePlot(QString filename)
{
    Vespucci::SavePlot(ui->customPlot, filename);
}

QColor PlotWidget::GetNextColor()
{
    //there are nine colors in the list
    //we alternate between them then rotate back to the first
    int color_index = ui->customPlot->graphCount();
    while (color_index >= colors_.size()){color_index -= colors_.size();}
    return colors_[color_index];
}
