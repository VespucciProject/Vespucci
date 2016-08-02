#include "abscissainterpolationdialog.h"
#include "ui_abscissainterpolationdialog.h"
AbscissaInterpolationDialog::AbscissaInterpolationDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::AbscissaInterpolationDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);

    ui->orderSpinBox->setVisible(false);
    ui->orderLabel->setVisible(false);
    ui->windowSpinBox->setVisible(false);
    ui->windowLabel->setVisible(false);

    ui->datasetComboBox->setVisible(true);
    ui->datasetLabel->setVisible(true);
    ui->fileLabel->setVisible(false);
    ui->browsePushButton->setVisible(false);
    ui->filenameLineEdit->setVisible(false);

    QStringList dataset_names = workspace_->dataset_names();
    ui->datasetComboBox->addItems(dataset_names);

}

AbscissaInterpolationDialog::~AbscissaInterpolationDialog()
{
    delete ui;
}


void AbscissaInterpolationDialog::on_buttonBox_accepted()
{
    /*
    arma::vec new_abscissa;
    if (ui->sourceComboBox->currentText() == "Dataset"){
        new_abscissa =
                workspace_->DatasetAtt)
                workspace_->DatasetAt(ui->datasetComboBox->currentIndex())->abscissa();
    }
    else if (ui->sourceComboBox->currentText() == "File"){
        arma::mat infile;
        bool ok = infile.load(ui->filenameLineEdit->text().toStdString());
        if (ok){
            new_abscissa = infile.col(0);
        }
        else{
            QMessageBox::warning(this, "Error Opening File",
                                 "The file " + ui->filenameLineEdit->text()\
                                 + " could not be opened");\
            return;
        }
    }
    else{dataset_.clear(); return;}
    QProgressDialog progress(this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setLabelText("Interpolating Spectra...");
    progress.setCancelButton(0);
    progress.setRange(0,0);
    progress.exec();
    if (ui->methodComboBox->currentText() == "Two-Point Linear"){
        try{
            dataset_->InterpolateToNewAbscissa(new_abscissa);
        }catch(exception e){
            progress.close();
            workspace_->main_window()->DisplayExceptionWarning(e);
        }
    }
    else if (ui->methodComboBox->currentText() == "Spline"){
        int poly_order = ui->orderSpinBox->value();
        int window_size = ui->windowSpinBox->value();
        try{
            dataset_->InterpolateToNewAbscissa(new_abscissa,
                                               poly_order,
                                               window_size);
        }catch(exception e){
            progress.close();
            workspace_->main_window()->DisplayExceptionWarning(e);
        }

    }
    else{
        progress.close();
        dataset_.clear();
        return;
    }
    progress.close();
    dataset_.clear();
*/
}

void AbscissaInterpolationDialog::on_sourceComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Dataset"){
        ui->datasetComboBox->setVisible(true);
        ui->datasetLabel->setVisible(true);
        ui->fileLabel->setVisible(false);
        ui->browsePushButton->setVisible(false);
        ui->filenameLineEdit->setVisible(false);
    }

    if (arg1 == "File"){
        ui->datasetComboBox->setVisible(false);
        ui->datasetLabel->setVisible(false);
        ui->fileLabel->setVisible(true);
        ui->browsePushButton->setVisible(true);
        ui->filenameLineEdit->setVisible(true);
    }
}

void AbscissaInterpolationDialog::on_methodComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Two-Point Linear"){
        ui->orderSpinBox->setVisible(false);
        ui->orderLabel->setVisible(false);
        ui->windowSpinBox->setVisible(false);
        ui->windowLabel->setVisible(false);
    }
    if (arg1 == "Spline"){
        ui->orderSpinBox->setVisible(true);
        ui->orderLabel->setVisible(true);
        ui->windowSpinBox->setVisible(true);
        ui->windowLabel->setVisible(true);
    }
}
