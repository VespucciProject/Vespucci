#include "GUI/Processing/abscissatransformdialog.h"
#include "ui_abscissatransformdialog.h"
#include <mlpack/core.hpp>

AbscissaTransformDialog::AbscissaTransformDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::AbscissaTransformDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);
    ui->descriptionLineEdit->setText(dataset_->x_axis_description());
}

AbscissaTransformDialog::~AbscissaTransformDialog()
{
    delete ui;

}

void AbscissaTransformDialog::on_buttonBox_accepted()
{
    double out_factor = ParseInput(ui->outFactorLineEdit->text());
    double in_factor = ParseInput(ui->inFactorLineEdit->text());
    QString out_units = ui->outUnitsComboBox->currentText();
    QString in_units = ui->inUnitsComboBox->currentText();
    QString description = ui->descriptionLineEdit->text();
    if (in_factor * out_factor){
        dataset_->TransformAbscissa(in_units, in_factor, out_units, out_factor, description);
    }
}

double AbscissaTransformDialog::ParseInput(QString text) const
{
    //split text into segments based on the asterix:
    text.remove(" "); //remove whitespace
    QStringList segments = text.split("*", QString::SkipEmptyParts);
    double factor = 1.0;
    foreach (const QString &segment, segments){
        factor *= TextToDouble(segment);
    }
    return factor;
}

double AbscissaTransformDialog::TextToDouble(QString text) const
{
    double val;
    bool ok;
    if (text == "eV"){val = arma::datum::eV; ok = true;}
    else{val = text.toDouble(&ok);}
    return (ok ? val : 0);
}
