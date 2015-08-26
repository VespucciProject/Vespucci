#include "fouriertransformdialog.h"
#include "ui_fouriertransformdialog.h"

FourierTransformDialog::FourierTransformDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::FourierTransformDialog)
{
    ui->setupUi(this);

    cut_double_spin_box_ = findChild<QDoubleSpinBox*>("cutDoubleSpinBox");
    cut_label_ = findChild<QLabel*>("cutLabel");
    spectrum_spin_box_ = findChild<QSpinBox*>("spectrumSpinBox");
    weights_combo_box_ = findChild<QComboBox*>("weightsComboBox");
    weights_label_ = findChild<QLabel *>("weightsLabel");
    parameter_double_spin_box_ = findChild<QDoubleSpinBox*>("parameterDoubleSpinBox");
    parameter_label_ = findChild<QLabel *>("parameterLabel");
    prediction_check_box_ = findChild<QCheckBox*>("predictionCheckBox");
    spectrum_custom_plot_ = findChild<QCustomPlot *>("spectrumCustomPlot");
}

FourierTransformDialog::~FourierTransformDialog()
{
    delete ui;
}



void FourierTransformDialog::on_typeComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Inverse FFT"){
        cut_double_spin_box_->setVisible(false);
        cut_label_->setVisible(false);
        weights_combo_box_->setVisible(false);
        weights_label_->setVisible(false);
        parameter_label_->setVisible(false);
        parameter_double_spin_box_->setVisible(false);
    }
    if (arg1 == "FFT"){
        cut_double_spin_box_->setVisible(true);
        cut_label_->setVisible(true);
        weights_combo_box_->setVisible(true);
        weights_label_->setVisible(true);
        parameter_label_->setVisible(true);
        parameter_double_spin_box_->setVisible(true);
    }
}

void FourierTransformDialog::on_weightsComboBox_currentIndexChanged(const QString &arg1)
{

}

void FourierTransformDialog::on_previewPushButton_clicked()
{

}
