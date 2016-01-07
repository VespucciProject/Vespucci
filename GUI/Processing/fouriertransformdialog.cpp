#include "fouriertransformdialog.h"
#include "ui_fouriertransformdialog.h"
#include "Global/global.h"
FourierTransformDialog::FourierTransformDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::FourierTransformDialog)
{
    ui->setupUi(this);
    workspace = ws;
    dataset_ = workspace->DatasetAt(row);

    end_double_spin_box_ = findChild<QDoubleSpinBox*>("endDoubleSpinBox");
    end_label_ = findChild<QLabel*>("endLabel");
    spectrum_spin_box_ = findChild<QSpinBox*>("spectrumSpinBox");
    weights_combo_box_ = findChild<QComboBox*>("weightsComboBox");
    weights_label_ = findChild<QLabel *>("weightsLabel");
    parameter_double_spin_box_ = findChild<QDoubleSpinBox*>("parameterDoubleSpinBox");
    parameter_label_ = findChild<QLabel *>("parameterLabel");
    spectrum_custom_plot_ = findChild<QCustomPlot *>("spectrumCustomPlot");
    sine_power_label_ = findChild<QLabel*>("sinePowerLabel");
    sine_power_double_spin_box_ = findChild<QDoubleSpinBox*>("sinePowerDoubleSpinBox");
    power_spin_box_ = findChild<QSpinBox*>("powerSpinBox");
    value_label_ = findChild<QLabel*>("valueLabel");
    type_combo_box_ = findChild<QComboBox*>("typeComboBox");

    parameter_double_spin_box_->setVisible(false);
    parameter_label_->setVisible(false);
    end_double_spin_box_->setVisible(false);
    end_label_->setVisible(false);
    sine_power_double_spin_box_->setVisible(false);
    sine_power_label_->setVisible(false);
    spectrum_spin_box_->setRange(1, dataset_->spectra_ptr()->n_cols);

    spectrum_custom_plot_->addGraph();

}

FourierTransformDialog::~FourierTransformDialog()
{
    delete ui;
}



void FourierTransformDialog::on_typeComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Inverse FFT"){
        end_double_spin_box_->setVisible(false);
        end_label_->setVisible(false);
        weights_combo_box_->setVisible(false);
        weights_label_->setVisible(false);
        parameter_label_->setVisible(false);
        parameter_double_spin_box_->setVisible(false);
    }
    QString weights = weights_combo_box_->currentText();
    if (arg1 == "FFT"){
        weights_combo_box_->setVisible(true);
        weights_label_->setVisible(true);
        if (weights == "Sine"){
            end_double_spin_box_->setVisible(true);
            end_label_->setVisible(true);
            end_label_->setText("Ending Offset");
            parameter_label_->setVisible(true);
            parameter_label_->setText("Starting Offset");
            sine_power_double_spin_box_->setVisible(true);
            sine_power_label_->setVisible(true);
        }
        else if (weights == "Gaussian"){
            parameter_double_spin_box_->setVisible(true);
            parameter_label_->setVisible(true);
            end_double_spin_box_->setVisible(false);
            end_label_->setVisible(false);
            sine_power_label_->setVisible(false);
            sine_power_double_spin_box_->setVisible(false);
            parameter_label_->setText("σ");
        }
        else if (weights == "Exponential"){
            parameter_double_spin_box_->setVisible(true);
            parameter_label_->setVisible(true);
            end_double_spin_box_->setVisible(false);
            end_label_->setVisible(false);
            sine_power_label_->setVisible(false);
            sine_power_double_spin_box_->setVisible(false);
            parameter_label_->setText("λ");
        }
        else{
            parameter_double_spin_box_->setVisible(false);
            parameter_label_->setVisible(false);
            end_double_spin_box_->setVisible(false);
            end_label_->setVisible(false);
            sine_power_double_spin_box_->setVisible(false);
            sine_power_label_->setVisible(false);
        }
    }
}

void FourierTransformDialog::on_weightsComboBox_currentIndexChanged(const QString &arg1)
{

    if (arg1 == "Sine Bell"){
        end_double_spin_box_->setVisible(true);
        end_label_->setVisible(true);
        end_label_->setText("Ending Offset");
        parameter_label_->setVisible(true);
        parameter_label_->setText("Starting Offset");
        sine_power_double_spin_box_->setVisible(true);
        sine_power_label_->setVisible(true);
    }
    else if (arg1 == "Gaussian"){
        parameter_double_spin_box_->setVisible(true);
        parameter_label_->setVisible(true);
        end_double_spin_box_->setVisible(false);
        end_label_->setVisible(false);
        sine_power_label_->setVisible(false);
        sine_power_double_spin_box_->setVisible(false);
        parameter_label_->setText("σ");
    }
    else if (arg1 == "Exponential"){
        parameter_double_spin_box_->setVisible(true);
        parameter_label_->setVisible(true);
        end_double_spin_box_->setVisible(false);
        end_label_->setVisible(false);
        sine_power_label_->setVisible(false);
        sine_power_double_spin_box_->setVisible(false);
        parameter_label_->setText("λ");
    }
    else{
        parameter_double_spin_box_->setVisible(false);
        parameter_label_->setVisible(false);
        end_double_spin_box_->setVisible(false);
        end_label_->setVisible(false);
        sine_power_double_spin_box_->setVisible(false);
        sine_power_label_->setVisible(false);
    }
}

void FourierTransformDialog::on_previewPushButton_clicked()
{
    typedef QVector<double> qvec;
    typedef std::vector<double> stdvec;
    qvec abs_qvec;
    qvec spc_qvec;

    arma::vec abscissa = dataset_->abscissa();
    arma::uvec ind = {(arma::uword) spectrum_spin_box_->value() - 1};
    arma::uword n = std::pow(2.0, power_spin_box_->value());

    if (type_combo_box_->currentText() == "FFT"){
        arma::cx_mat f_spectrum;
        arma::vec f_abscissa;
        arma::mat spectrum = dataset_->spectra(ind);
        try{
            Vespucci::Math::Transform::fft_mat(spectrum, abscissa, f_spectrum, f_abscissa, n);
            abs_qvec = qvec::fromStdVector(arma::conv_to<stdvec>::from(f_abscissa));
            spc_qvec = qvec::fromStdVector(arma::conv_to<stdvec>::from(arma::real(f_spectrum)));
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
            close();
        }
    }
    else if (type_combo_box_->currentText() == "Inverse FFT"){
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
            workspace->main_window()->DisplayExceptionWarning(e);
            close();
        }
    }
    else{return;}

    spectrum_custom_plot_->graph(0)->setData(abs_qvec, spc_qvec);
    spectrum_custom_plot_->replot();

}


void FourierTransformDialog::on_powerSpinBox_valueChanged(int arg1)
{
    value_label_->setText("= " + QString::number( std::pow(2.0, arg1) ) );
}

void FourierTransformDialog::on_weightsPushButton_clicked()
{
    QString type = weights_combo_box_->currentText();
    int response = QMessageBox::warning(this, "Apply Weights",
                         "Applying weights will change the spectra permanently."
                         "This can be undone, but only once. Are you sure you "
                         "want to proceed?",
                         QMessageBox::Ok, QMessageBox::Cancel);
    if (response == QMessageBox::Cancel){return;}
    if (type == "Sine Bell"){
        dataset_->ApplyFTWeight(parameter_double_spin_box_->value(),
                                end_double_spin_box_->value(),
                                sine_power_double_spin_box_->value());
    }
    else if (type == "Exponential"
             || type == "Gaussian"){
        dataset_->ApplyFTWeight(type, parameter_double_spin_box_->value());
    }
    else{
        return;
    }
}

void FourierTransformDialog::on_buttonBox_accepted()
{
    int n = std::pow(2, power_spin_box_->value());
    cout << "n = " << n << endl;

    if (type_combo_box_->currentText() == "FFT"){
        QProgressDialog *progress =
                Vespucci::DisplayProgressDialog(this,
                                                "Performing FFT",
                                                "Performing FFT...");
        try{
            dataset_->FourierTransform(n);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
            progress->cancel();
        }
        progress->cancel();
    }
    else if (type_combo_box_->currentText() == "Inverse FFT"){
        QProgressDialog *progress =
                Vespucci::DisplayProgressDialog(this,
                                                "Performing Inverse FFT",
                                                "Performing Inverse FFT...");
        try{
            dataset_->InverseFourierTransform(n);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
            progress->cancel();
        }
        progress->cancel();
    }
    else{close();}
    close();
}
