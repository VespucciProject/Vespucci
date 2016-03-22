#include "GUI/Display/plotviewer.h"
#include "GUI/Display/plotviewer.h"
#include "ui_plotviewer.h"
#include "Global/global.h"
class PlotWidget;
PlotViewer::PlotViewer(MainWindow *parent) :
    QDialog(0),
    ui(new Ui::PlotViewer)
{
    ui->setupUi(this);
    tab_widget_ = findChild<QTabWidget*>("tabWidget");
    stack_check_box_ = findChild<QCheckBox*>("stackCheckBox");
    hold_check_box_ = findChild<QCheckBox*>("holdCheckBox");
    //plotviewer is initialized with empty tab widget but not set visible until
    //a tab widget is added via a plot command
    while (tab_widget_->count()){tab_widget_->removeTab(0);}
}

PlotViewer::~PlotViewer()
{
    delete ui;
}

void PlotViewer::AddPlot(const mat &paired_data, const QString &tab_title)
{
    if (hold_check_box_->isChecked() && tab_widget_->count()){
        PlotWidget *plot_widget = qobject_cast<PlotWidget *>(tab_widget_->currentWidget());
        plot_widget->AddPlot(paired_data);
        plot_widget->StackPlots(stack_check_box_->isChecked());
    }
    else{
        PlotWidget *plot_widget = new PlotWidget(this);
        plot_widget->AddPlot(paired_data);
        tab_widget_->addTab(plot_widget, tab_title);
    }
}

void PlotViewer::AddPlot(const vec &abscissa, const vec &data, const QString &tab_title)
{
    if (hold_check_box_->isChecked() && tab_widget_->count()){
        PlotWidget *plot_widget = qobject_cast<PlotWidget *>(tab_widget_->currentWidget());
        plot_widget->AddPlot(abscissa, data);
        plot_widget->StackPlots(stack_check_box_->isChecked());
    }
    else{
        PlotWidget *plot_widget = new PlotWidget(this);
        plot_widget->AddPlot(abscissa, data);
        tab_widget_->addTab(plot_widget, tab_title);
    }
}


void PlotViewer::on_stackCheckBox_stateChanged(int arg1)
{
    bool stack_plots;
    stack_plots = (arg1 == Qt::Checked);
    if (tab_widget_->count()){
        PlotWidget *plot_widget = qobject_cast<PlotWidget *>(tab_widget_->currentWidget());
        plot_widget->StackPlots(stack_plots);
    }
}

void PlotViewer::on_tabWidget_currentChanged(int index)
{
    PlotWidget *plot_widget = qobject_cast<PlotWidget *>(tab_widget_->currentWidget());
    stack_check_box_->setChecked(plot_widget->offset_plots());
}

void PlotViewer::on_tabWidget_tabCloseRequested(int index)
{
    tab_widget_->removeTab(index);
}
