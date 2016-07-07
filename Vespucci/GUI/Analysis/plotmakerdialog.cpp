#include "GUI/Analysis/plotmakerdialog.h"
#include "ui_plotmakerdialog.h"
#include "GUI/Display/plotviewer.h"

PlotMakerDialog::PlotMakerDialog(MainWindow *parent, QSharedPointer<VespucciWorkspace> ws, const mat & data) :
    QDialog(parent),
    ui(new Ui::PlotMakerDialog),
    data_(data)
{
    ui->setupUi(this);
    workspace_ = ws;

}

PlotMakerDialog::~PlotMakerDialog()
{
    delete ui;
}

void PlotMakerDialog::on_buttonBox_accepted()
{

}
