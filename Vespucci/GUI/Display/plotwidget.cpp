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

PlotWidget::PlotWidget(QWidget *parent, VespucciWorkspace *ws) :
    QWidget(parent),
    ui(new Ui::PlotWidget)
{
    workspace = ws;
    ui->setupUi(this);
    plot_ = findChild<QCustomPlot *>("customPlot");
    plot_->setInteraction(QCP::iRangeDrag);
    plot_->setInteraction(QCP::iRangeZoom);
    plot_->setInteraction(QCP::iSelectItems);
    plot_->setInteraction(QCP::iMultiSelect);
    plot_->setInteraction(QCP::iSelectLegend);
    plot_->setInteraction(QCP::iSelectOther);
    plot_->setInteraction(QCP::iSelectPlottables);
    offset_by_ = 0;

    colors_.append(QColor(228,26,28));
    colors_.append(QColor(55,126,184));
    colors_.append(QColor(77,175,74));
    colors_.append(QColor(152,78,163));
    colors_.append(QColor(255,127,0));
    colors_.append(QColor(255,255,51));
    colors_.append(QColor(166,86,40));
    colors_.append(QColor(247,129,191));
    colors_.append(QColor(153,153,153));
}

PlotWidget::~PlotWidget()
{
    delete ui;
}

void PlotWidget::AddPlot(const mat &paired_data)
{
    if (paired_data.n_cols < 2)
        return;
    QVector<double> abscissa =
            QVector<double>::fromStdVector(conv_to<stdvec>::from(paired_data.col(0)));
    QVector<double> data =
            QVector<double>::fromStdVector(conv_to<stdvec>::from(paired_data.col(1)));
    int graph_count = plot_->graphCount();
    if (graph_count < 1){
        plot_->addGraph();
        plot_->graph(graph_count)->addData(abscissa, data);
    }
    else{
        plot_->addGraph(plot_->graph(0)->keyAxis(), plot_->graph(0)->valueAxis());
        if (offset_plots_){
            RemoveOffset();
            DetermineOffset(data);
            plot_->graph(graph_count)->addData(abscissa, data);
            ApplyOffset();
        }
        else{
            plot_->graph(graph_count)->addData(abscissa, data);
        }

    }
}

void PlotWidget::StackPlots(bool stack)
{
    if (plot_->graphCount() < 1){
        offset_plots_ = true;
        return;
    }
    if (offset_plots_ && !stack){
        RemoveOffset();
    }
    if (!offset_plots_ && stack){
        RemoveOffset();
        DetermineOffset();
        ApplyOffset();
    }

    offset_plots_ = stack;

}

bool PlotWidget::offset_plots()
{
    return offset_plots_;
}



void PlotWidget::DetermineOffset()
{
    if (!plot_->graphCount()){
        offset_by_ = 0;
        return;
    }
    stdvec first_y = GetData(plot_->graph(0)->data()->values()).toStdVector();
    double min = *min_element(first_y.begin(), first_y.end());
    double max = *max_element(first_y.begin(), first_y.end());
    for (int i = 1; i < plot_->graphCount(); ++i){
        stdvec y = GetData(plot_->graph(i)->data()->values()).toStdVector();
        double current_min = *min_element(y.begin(), y.end());
        double current_max = *max_element(y.begin(), y.end());
        min = current_min < min ? current_min : min;
        max = current_max > max ? current_max : max;
    }
    //increase offset by 10% to prevent graphs touching
    offset_by_ = 1.1*fabs(max - min);
}

void PlotWidget::DetermineOffset(const QVector<double> &new_data)
{
    //determine starting values for min and max
    stdvec new_y = new_data.toStdVector();
    double min = *min_element(new_y.begin(), new_y.end());
    double max = *max_element(new_y.begin(), new_y.end());

    for (int i = 0; i < plot_->graphCount(); ++i){
        stdvec y = GetData(plot_->graph(i)->data()->values()).toStdVector();
        double current_min = *min_element(y.begin(), y.end());
        double current_max = *max_element(y.begin(), y.end());
        min = std::min(current_min, min);
        max = std::max(current_max, max);
    }
    offset_by_ = 1.1*fabs(max - min);
}

void PlotWidget::ApplyOffset()
{
    for (int i = 0; i < plot_->graphCount(); ++i){
        QVector<double> data = GetData(plot_->graph(i)->data()->values());
        QVector<double> abscissa = GetAbscissa(plot_->graph(i)->data()->values());
        for (int i = 0; i < data.size(); ++i){data[i] = data[i] + offset_by_;}
        plot_->graph(i)->setData(abscissa, data);
    }
}

void PlotWidget::RemoveOffset()
{
    for (int i = 0; i < plot_->graphCount(); ++i){
        QVector<double> data = GetData(plot_->graph(i)->data()->values());
        QVector<double> abscissa = GetAbscissa(plot_->graph(i)->data()->values());
        for (int i = 0; i < data.size(); ++i){data[i] = data[i] - offset_by_;}
        plot_->graph(i)->setData(abscissa, data);
    }
}

QVector<double> PlotWidget::GetData(const QList<QCPData> &values) const
{
    QVector<double> data;
    foreach(QCPData value, values){data.push_back(value.value);}
    return data;
}

QVector<double> PlotWidget::GetAbscissa(const QList<QCPData> &keys) const
{
    QVector<double> values;
    foreach(QCPData data, keys){values.push_back(data.key);}
    return values;
}

QColor PlotWidget::GetNextColor()
{
    //there are nine colors in the list
    //we alternate between them then rotate back to the first
    int color_index = plot_->graphCount();
    while (color_index > 8){color_index -= 9;}
    return colors_[color_index];
}
