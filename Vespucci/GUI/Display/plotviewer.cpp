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
    workspace_ = parent->workspace_ptr();
}

PlotViewer::~PlotViewer()
{
    delete ui;
}

void PlotViewer::AddPlot(const mat & paired_data, const QString &tab_title)
{
    if (ui->holdCheckBox->isChecked() && ui->tabWidget->count()){
        PlotWidget *plot_widget = qobject_cast<PlotWidget *>(ui->tabWidget->currentWidget());
        plot_widget->AddPlot(paired_data);
        plot_widget->StackPlots(ui->stackCheckBox->isChecked());
    }
    else{
        PlotWidget *plot_widget = new PlotWidget(this, workspace_);
        plot_widget->AddPlot(paired_data);
        ui->tabWidget->addTab(plot_widget, tab_title);
    }
}

void PlotViewer::AddPlot(const vec &abscissa, const vec &data, const QString &tab_title)
{
    if (ui->holdCheckBox->isChecked() && ui->tabWidget->count()){
        PlotWidget *plot_widget = qobject_cast<PlotWidget *>(ui->tabWidget->currentWidget());
        plot_widget->AddPlot(abscissa, data);
        plot_widget->StackPlots(ui->stackCheckBox->isChecked());
    }
    else{
        PlotWidget *plot_widget = new PlotWidget(this, workspace_);
        plot_widget->AddPlot(abscissa, data);
        ui->tabWidget->addTab(plot_widget, tab_title);
    }
}

void PlotViewer::AddTransientPlot(const vec &abscissa, const vec &data, const QString &tab_title)
{
    if (ui->holdCheckBox->isChecked() && ui->tabWidget->count()){
        PlotWidget *plot_widget = qobject_cast<PlotWidget*>(ui->tabWidget->currentWidget());
        plot_widget->AddTransientPlot(abscissa, data);
        plot_widget->StackPlots(ui->stackCheckBox->isChecked());
    }
    else{
        PlotWidget *plot_widget = new PlotWidget(this, workspace_);
        plot_widget->AddTransientPlot(abscissa, data);
        ui->tabWidget->addTab(plot_widget, tab_title);
    }
}

void PlotViewer::AddTransientPlot(const mat & paired_data, const QString &tab_title)
{
    if (ui->holdCheckBox->isChecked() && ui->tabWidget->count()){
        PlotWidget *plot_widget = qobject_cast<PlotWidget*>(ui->tabWidget->currentWidget());
        plot_widget->AddTransientPlot(paired_data);
        plot_widget->StackPlots(ui->stackCheckBox->isChecked());
    }
    else{
        PlotWidget *plot_widget = new PlotWidget(this, workspace_);
        plot_widget->AddTransientPlot(paired_data);
        ui->tabWidget->addTab(plot_widget, tab_title);
    }
}

void PlotViewer::CloseTransientTab()
{
    PlotWidget *plot_widget = qobject_cast<PlotWidget*>(ui->tabWidget->currentWidget());
    if (plot_widget->TransientOnly()) ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
}

void PlotViewer::closeEvent(QCloseEvent *ev)
{
    QDialog::closeEvent(ev);
    emit SetActionChecked(false);
}


void PlotViewer::on_stackCheckBox_stateChanged(int arg1)
{
    bool stack_plots;
    stack_plots = (arg1 == Qt::Checked);
    if (ui->tabWidget->count()){
        PlotWidget *plot_widget = qobject_cast<PlotWidget *>(ui->tabWidget->currentWidget());
        plot_widget->StackPlots(stack_plots);
    }
}

void PlotViewer::on_tabWidget_currentChanged(int index)
{
    if (!ui->tabWidget->currentWidget()){return;}
    PlotWidget *plot_widget = qobject_cast<PlotWidget *>(ui->tabWidget->currentWidget());
    ui->stackCheckBox->setChecked(plot_widget->offset_plots());
}

void PlotViewer::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}
