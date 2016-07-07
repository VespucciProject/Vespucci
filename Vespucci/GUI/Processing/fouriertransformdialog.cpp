#include "fouriertransformdialog.h"
#include "ui_fouriertransformdialog.h"
#include "Global/global.h"
FourierTransformDialog::FourierTransformDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::FourierTransformDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);

    ui->parameterDoubleSpinBox->setVisible(false);
    ui->parameterLabel->setVisible(false);
    ui->endDoubleSpinBox->setVisible(false);
    ui->endLabel->setVisible(false);
    ui->sinePowerDoubleSpinBox->setVisible(false);
    ui->sinePowerLabel->setVisible(false);
    ui->spectrumSpinBox->setRange(1, dataset_->spectra_ptr()->n_cols);

    ui->spectrumCustomPlot->addGraph();

}

FourierTransformDialog::~FourierTransformDialog()
{
    delete ui;
}



void FourierTransformDialog::on_typeComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Inverse FFT"){
        ui->endDoubleSpinBox->setVisible(false);
        ui->endLabel->setVisible(false);
        ui->weightsComboBox->setVisible(false);
        ui->weightsLabel->setVisible(false);
        ui->parameterLabel->setVisible(false);
        ui->parameterDoubleSpinBox->setVisible(false);
    }
    QString weights = ui->weightsComboBox->currentText();
    if (arg1 == "FFT"){
        ui->weightsComboBox->setVisible(true);
        ui->weightsLabel->setVisible(true);
        if (weights == "Sine"){
            ui->endDoubleSpinBox->setVisible(true);
            ui->endLabel->setVisible(true);
            ui->endLabel->setText("Ending Offset");
            ui->parameterLabel->setVisible(true);
            ui->parameterLabel->setText("Starting Offset");
            ui->sinePowerDoubleSpinBox->setVisible(true);
            ui->sinePowerLabel->setVisible(true);
        }
        else if (weights == "Gaussian"){
            ui->parameterDoubleSpinBox->setVisible(true);
            ui->parameterLabel->setVisible(true);
            ui->endDoubleSpinBox->setVisible(false);
            ui->endLabel->setVisible(false);
            ui->sinePowerLabel->setVisible(false);
            ui->sinePowerDoubleSpinBox->setVisible(false);
            ui->parameterLabel->setText("σ");
        }
        else if (weights == "Exponential"){
            ui->parameterDoubleSpinBox->setVisible(true);
            ui->parameterLabel->setVisible(true);
            ui->endDoubleSpinBox->setVisible(false);
            ui->endLabel->setVisible(false);
            ui->sinePowerLabel->setVisible(false);
            ui->sinePowerDoubleSpinBox->setVisible(false);
            ui->parameterLabel->setText("λ");
        }
        else{
            ui->parameterDoubleSpinBox->setVisible(false);
            ui->parameterLabel->setVisible(false);
            ui->endDoubleSpinBox->setVisible(false);
            ui->endLabel->setVisible(false);
            ui->sinePowerDoubleSpinBox->setVisible(false);
            ui->sinePowerLabel->setVisible(false);
        }
    }
}

void FourierTransformDialog::on_weightsComboBox_currentIndexChanged(const QString &arg1)
{

    if (arg1 == "Sine Bell"){
        ui->endDoubleSpinBox->setVisible(true);
        ui->endLabel->setVisible(true);
        ui->endLabel->setText("Ending Offset");
        ui->parameterLabel->setVisible(true);
        ui->parameterLabel->setText("Starting Offset");
        ui->sinePowerDoubleSpinBox->setVisible(true);
        ui->sinePowerLabel->setVisible(true);
    }
    else if (arg1 == "Gaussian"){
        ui->parameterDoubleSpinBox->setVisible(true);
        ui->parameterLabel->setVisible(true);
        ui->endDoubleSpinBox->setVisible(false);
        ui->endLabel->setVisible(false);
        ui->sinePowerLabel->setVisible(false);
        ui->sinePowerDoubleSpinBox->setVisible(false);
        ui->parameterLabel->setText("σ");
    }
    else if (arg1 == "Exponential"){
        ui->parameterDoubleSpinBox->setVisible(true);
        ui->parameterLabel->setVisible(true);
        ui->endDoubleSpinBox->setVisible(false);
        ui->endLabel->setVisible(false);
        ui->sinePowerLabel->setVisible(false);
        ui->sinePowerDoubleSpinBox->setVisible(false);
        ui->parameterLabel->setText("λ");
    }
    else{
        ui->parameterDoubleSpinBox->setVisible(false);
        ui->parameterLabel->setVisible(false);
        ui->endDoubleSpinBox->setVisible(false);
        ui->endLabel->setVisible(false);
        ui->sinePowerDoubleSpinBox->setVisible(false);
        ui->sinePowerLabel->setVisible(false);
    }
}

void FourierTransformDialog::on_previewPushButton_clicked()
{
    typedef QVector<double> qvec;
    typedef std::vector<double> stdvec;
    qvec abs_qvec;
    qvec spc_qvec;

    arma::vec abscissa = dataset_->abscissa();
    arma::uvec ind = {(arma::uword) ui->spectrumSpinBox->value() - 1};
    arma::uword n = std::pow(2.0, ui->powerSpinBox->value());

    if (ui->typeComboBox->currentText() == "FFT"){
        arma::cx_mat f_spectrum;
        arma::vec f_abscissa;
        arma::mat spectrum = dataset_->spectra(ind);
        try{
            Vespucci::Math::Transform::fft_mat(spectrum, abscissa, f_spectrum, f_abscissa, n);
            abs_qvec = qvec::fromStdVector(arma::conv_to<stdvec>::from(f_abscissa));
            spc_qvec = qvec::fromStdVector(arma::conv_to<stdvec>::from(arma::real(f_spectrum)));
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
            close();
        }
    }
    else if (ui->typeComboBox->currentText() == "Inverse FFT"){
        arma::cx_mat t_spectrum;
        arma::vec t_abscissa;
        arma::cx_mat cx_spectrum = dataset_->cx_spectra(ind);

        try{
            Vespucci::Math::Transform::ifft_mat(cx_spectrum, abscissa,
                                                t_spectrum, t_abscissa,
                                                n);
            abs_qvec =
                    qvec::fromStdVector(arma::conv_to<stdvec>::from(t_abscissa));
            spc_qvec =
                    qvec::fromStdVector(arma::conv_to<stdvec>::from(arma::real(t_spectrum)));
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
            close();
        }
    }
    else{return;}

    ui->spectrumCustomPlot->graph(0)->setData(abs_qvec, spc_qvec);
    ui->spectrumCustomPlot->replot();

}


void FourierTransformDialog::on_powerSpinBox_valueChanged(int arg1)
{
    ui->valueLabel->setText("= " + QString::number( std::pow(2.0, arg1) ) );
}

void FourierTransformDialog::on_weightsPushButton_clicked()
{
    QString type = ui->weightsComboBox->currentText();
    int response = QMessageBox::warning(this, "Apply Weights",
                         "Applying weights will change the spectra permanently."
                         "This can be undone, but only once. Are you sure you "
                         "want to proceed?",
                         QMessageBox::Ok, QMessageBox::Cancel);
    if (response == QMessageBox::Cancel){return;}
    if (type == "Sine Bell"){
        dataset_->ApplyFTWeight(ui->parameterDoubleSpinBox->value(),
                                ui->endDoubleSpinBox->value(),
                                ui->sinePowerDoubleSpinBox->value());
    }
    else if (type == "Exponential"
             || type == "Gaussian"){
        dataset_->ApplyFTWeight(type, ui->parameterDoubleSpinBox->value());
    }
    else{
        return;
    }
}

void FourierTransformDialog::on_buttonBox_accepted()
{
    int n = std::pow(2, ui->powerSpinBox->value());
    cout << "n = " << n << endl;

    if (ui->typeComboBox->currentText() == "FFT"){
        QProgressDialog *progress =
                Vespucci::DisplayProgressDialog(this,
                                                "Performing FFT",
                                                "Performing FFT...");
        try{
            dataset_->FourierTransform(n);
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
            progress->cancel();
        }
        progress->cancel();
    }
    else if (ui->typeComboBox->currentText() == "Inverse FFT"){
        QProgressDialog *progress =
                Vespucci::DisplayProgressDialog(this,
                                                "Performing Inverse FFT",
                                                "Performing Inverse FFT...");
        try{
            dataset_->InverseFourierTransform(n);
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
            progress->cancel();
        }
        progress->cancel();
    }
    else{close();}
    close();
}
