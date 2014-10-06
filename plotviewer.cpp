#include "plotviewer.h"
#include "ui_plotviewer.h"

PlotViewer::PlotViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotViewer)
{
    ui->setupUi(this);
}

PlotViewer::~PlotViewer()
{
    delete ui;
}
