#include "GUI/Analysis/plotmakerdialog.h"
#include "ui_plotmakerdialog.h"
#include "GUI/Display/plotviewer.h"

PlotMakerDialog::PlotMakerDialog(MainWindow *parent, PlotViewer *plot_viewer, QSharedPointer<VespucciWorkspace> ws, QStringList data_keys) :
    QDialog(parent),
    ui(new Ui::PlotMakerDialog),
    data_keys_(data_keys)
{
    ui->setupUi(this);
    workspace_ = ws;
    parent_ = parent;
    plot_viewer_ = plot_viewer;

    QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(data_keys.first());
    if (dataset.isNull()){
        QMessageBox::warning(this, "Dataset Does not Exist",
                             "An error has occurred: dataset does not exist with specified name");
        close();
    }

    if (!workspace_->GetMatrix(data_keys).n_elem){
        QMessageBox::warning(this, "Matrix Does Not Exist",
                             "An error has occurred: matrix does not exist with specified keys");
        close();
    }


    if (workspace_->GetMatrix(data_keys).n_rows == dataset->abscissa_ref().n_rows)
        ui->abscissaComboBox->addItem("Spectral Abscissa");
    ui->abscissaComboBox->addItem("Column");

    ui->xSpinBox->setMaximum(workspace_->GetMatrix(data_keys_).n_cols);
    ui->ySpinBox->setMaximum(workspace_->GetMatrix(data_keys_).n_cols);
    ui->xSpinBox->setMinimum(1);
    ui->ySpinBox->setMinimum(1);
}

PlotMakerDialog::~PlotMakerDialog()
{
    delete ui;
}

void PlotMakerDialog::on_buttonBox_accepted()
{

    QString abscissa_type = ui->abscissaComboBox->currentText();
    QString dimension_type = ui->dimensionComboBox->currentText();
    uword x_ind = ui->xSpinBox->value() - 1;
    uword y_ind = ui->ySpinBox->value() - 1;
    vec x, y;



    QString plot_title = data_keys_.last()
            + " " + QString::number(y_ind);
    if (abscissa_type == "Row" || abscissa_type == "Column"){
        plot_title = plot_title
                + " vs. "
                + abscissa_type
                + " " + QString::number(x_ind);
    }

    if (dimension_type == "Column")
        y = workspace_->GetMatrix(data_keys_).col(y_ind);

    if (dimension_type == "Row")
        y = workspace_->GetMatrix(data_keys_).row(y_ind).t();

    if (abscissa_type == "Spectral Abscissa")
        x = workspace_->GetDataset(data_keys_.first())->abscissa();

    if (abscissa_type == "Column")
        x = workspace_->GetMatrix(data_keys_).col(x_ind);

    if (abscissa_type == "Row")
        x = workspace_->GetMatrix(data_keys_).row(x_ind).t();

    if (y.n_elem && (x.n_rows == y.n_rows))
        plot_viewer_->AddPlot(x, y, plot_title);
    else
        QMessageBox::warning(this, "Dimension Mismatch", "Matrix dimensions do not match");

    close();
}


void PlotMakerDialog::on_dimensionComboBox_currentTextChanged(const QString &arg1)
{
   if (arg1 == "Row"){
       ui->abscissaComboBox->clear();
       QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(data_keys_.first());
       if (workspace_->GetMatrix(data_keys_).n_cols == dataset->spectra_ref().n_rows)
           ui->abscissaComboBox->addItem("Spectral Abscissa");
       ui->abscissaComboBox->addItem("Row");
       ui->xSpinBox->setMaximum(workspace_->GetMatrix(data_keys_).n_rows);
       ui->ySpinBox->setMaximum(workspace_->GetMatrix(data_keys_).n_rows);
   }

   if (arg1 == "Column"){
       ui->abscissaComboBox->clear();
       QSharedPointer<VespucciDataset> dataset = workspace_->GetDataset(data_keys_.first());
       if (workspace_->GetMatrix(data_keys_).n_rows == dataset->abscissa_ref().n_rows)
           ui->abscissaComboBox->addItem("Spectral Abscissa");
       ui->abscissaComboBox->addItem("Column");
       ui->xSpinBox->setMaximum(workspace_->GetMatrix(data_keys_).n_cols);
       ui->ySpinBox->setMaximum(workspace_->GetMatrix(data_keys_).n_cols);
   }
}
