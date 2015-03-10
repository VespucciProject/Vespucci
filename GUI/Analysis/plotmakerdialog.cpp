#include "GUI/Analysis/plotmakerdialog.h"
#include "GUI/Analysis/ui_plotmakerdialog.h"

PlotMakerDialog::PlotMakerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotMakerDialog)
{
    ui->setupUi(this);
}

PlotMakerDialog::~PlotMakerDialog()
{
    delete ui;
}
