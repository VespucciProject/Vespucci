#include "GUI/Display/plotviewer.h"
#include "GUI/Display/plotviewer.h"
#include "ui_plotviewer.h"
#include "Global/global.h"
class PlotWidget;
PlotViewer::PlotViewer(MainWindow *parent, QSharedPointer<VespucciWorkspace> workspace) :
    QDockWidget(parent),
    ui(new Ui::PlotViewer)
{
    ui->setupUi(this);
    workspace_ = workspace;
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
    }
    else{
        PlotWidget *plot_widget = new PlotWidget(this, workspace_);
        plot_widget->AddPlot(paired_data);
        ui->tabWidget->addTab(plot_widget, tab_title);
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    }
}

void PlotViewer::AddPlot(const vec &abscissa, const vec &data, const QString &tab_title)
{
    if (ui->holdCheckBox->isChecked() && ui->tabWidget->count()){
        PlotWidget *plot_widget = qobject_cast<PlotWidget *>(ui->tabWidget->currentWidget());
        plot_widget->AddPlot(abscissa, data);
    }
    else{
        PlotWidget *plot_widget = new PlotWidget(this, workspace_);
        plot_widget->AddPlot(abscissa, data);
        ui->tabWidget->addTab(plot_widget, tab_title);
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    }
}

void PlotViewer::AddTransientPlot(const vec &abscissa, const vec &data, const QString &tab_title)
{
    if (ui->holdCheckBox->isChecked() && ui->tabWidget->count()){
        PlotWidget *plot_widget = qobject_cast<PlotWidget*>(ui->tabWidget->currentWidget());
        plot_widget->AddTransientPlot(abscissa, data);
    }
    else{
        PlotWidget *plot_widget = new PlotWidget(this, workspace_);
        plot_widget->AddTransientPlot(abscissa, data);
        ui->tabWidget->addTab(plot_widget, tab_title);
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    }
}

void PlotViewer::AddTransientPlot(const mat & paired_data, const QString &tab_title)
{
    if (ui->holdCheckBox->isChecked() && ui->tabWidget->count()){
        PlotWidget *plot_widget = qobject_cast<PlotWidget*>(ui->tabWidget->currentWidget());
        plot_widget->AddTransientPlot(paired_data);
    }
    else{
        PlotWidget *plot_widget = new PlotWidget(this, workspace_);
        plot_widget->AddTransientPlot(paired_data);
        ui->tabWidget->addTab(plot_widget, tab_title);
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    }
}

void PlotViewer::AddScatterPlot(const mat &paired_data, const QString &tab_title)
{    if (ui->holdCheckBox->isChecked() && ui->tabWidget->count()){
        PlotWidget *plot_widget = qobject_cast<PlotWidget *>(ui->tabWidget->currentWidget());
        plot_widget->AddPlot(paired_data);
    }
    else{
        PlotWidget *plot_widget = new PlotWidget(this, workspace_);
        plot_widget->AddScatterPlot(paired_data);
        ui->tabWidget->addTab(plot_widget, tab_title);
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    }

}

void PlotViewer::AddScatterPlot(const vec &abscissa, const vec &data, const QString &tab_title)
{
    if (ui->holdCheckBox->isChecked() && ui->tabWidget->count()){
        PlotWidget *plot_widget = qobject_cast<PlotWidget *>(ui->tabWidget->currentWidget());
        plot_widget->AddPlot(abscissa, data);
    }
    else{
        PlotWidget *plot_widget = new PlotWidget(this, workspace_);
        plot_widget->AddScatterPlot(abscissa, data);
        ui->tabWidget->addTab(plot_widget, tab_title);
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    }
}

///
/// \brief PlotViewer::AddTab
/// \param tab_title
/// Adds a tab without setting its data. Will ignore the hold checkbox
/// If the tab is meant to be transient, use CloseTransientTab to close it.
void PlotViewer::AddTab(const QString &tab_title)
{
    PlotWidget *plot_widget = new PlotWidget(this, workspace_);
    ui->tabWidget->addTab(plot_widget, tab_title);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void PlotViewer::SetHoldCheckBoxChecked(bool checked)
{
    ui->holdCheckBox->setChecked(checked);
}

void PlotViewer::CloseTransientTab()
{
    PlotWidget *plot_widget = qobject_cast<PlotWidget*>(ui->tabWidget->currentWidget());
    if (plot_widget->TransientOnly()) ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
}

void PlotViewer::closeEvent(QCloseEvent *ev)
{
    QDockWidget::closeEvent(ev);
    emit SetActionChecked(false);
}

void PlotViewer::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}


void PlotViewer::on_exportPushButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Export Plot",
                                                    workspace_->directory(),
                                                    "Tagged Image File Format (*.tif);;"
                                                    "Portable Network Graphics (*.png);;"
                                                    "Joint Photographic Experts Group (*.jpg);;"
                                                    "Scalable Vector Graphics (*.svg);;"
                                                    "Windows Bitmap (*.bmp)");
    PlotWidget *plot_widget = qobject_cast<PlotWidget *>(ui->tabWidget->currentWidget());
    plot_widget->SavePlot(filename);
}
