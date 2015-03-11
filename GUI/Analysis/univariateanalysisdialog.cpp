#include "GUI/Analysis/univariateanalysisdialog.h"
#include "ui_univariateanalysisdialog.h"

UnivariateAnalysisDialog::UnivariateAnalysisDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::UnivariateAnalysisDialog)
{
    ui->setupUi(this);
    first_min_box_ = this->findChild<QLineEdit *>("firstMinLineEdit_2");
    first_max_box_ = this->findChild<QLineEdit *>("firstMaxLineEdit_2");
    second_min_box_ = this->findChild<QLineEdit *>("secondMinLineEdit_2");
    second_max_box_ = this->findChild<QLineEdit *>("secondMaxLineEdit_2");
    integration_method_selector_ = this->findChild<QComboBox *>("integrationComboBox");
    range_label_ = this->findChild<QLabel *>("rangeLabel_2");
    value_method_selector_ = this->findChild<QComboBox *>("peakComboBox");

    workspace = ws;
    data_ = workspace->DatasetAt(row);

}

UnivariateAnalysisDialog::~UnivariateAnalysisDialog()
{
    delete ui;
}

void UnivariateAnalysisDialog::on_peakComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Area Ratio" || arg1 == "Intensity Ratio"){
        second_min_box_->setEnabled(true);
        second_max_box_->setEnabled(true);
    }
    else{
        second_min_box_->setEnabled(false);
        second_max_box_->setEnabled(false);
    }

    if (arg1 == "Area" || arg1 == "Area Ratio")
        integration_method_selector_->setEnabled(true);
    else
        integration_method_selector_->setEnabled(false);

}

void UnivariateAnalysisDialog::on_buttonBox_accepted()
{
    QString method_description = value_method_selector_->currentText();
    QString integration_method = integration_method_selector_->currentText();
    UnivariateMethod::Method method;

    QString name = "Univariate " + method_description
            + " " + QString::number(data_->UnivariateCount());

    if (method_description == "Area")
        method = UnivariateMethod::Area;
    else if (method_description == "Area Ratio")
        method = UnivariateMethod::AreaRatio;
    else if (method_description == "Bandwidth")
        method = UnivariateMethod::FWHM;
    else if (method_description == "Intensity Ratio")
        method = UnivariateMethod::IntensityRatio;
    else
        method = UnivariateMethod::Intensity;

    double first_min, first_max, second_min, second_max;
    bool ok;
    first_min = first_min_box_->text().toDouble(&ok);
    if(!ok){
        QMessageBox::warning(this, "Invalid Input", "First minimum must be a number");
        return;
    }
    first_max = first_max_box_->text().toDouble(&ok);
    if(!ok){
        QMessageBox::warning(this, "Invalid Input", "First maximum must be a number");
        return;
    }

    switch(method){
    case UnivariateMethod::AreaRatio: case UnivariateMethod::IntensityRatio:
        second_min = second_min_box_->text().toDouble(&ok);
        if(!ok){
            QMessageBox::warning(this, "Invalid Input", "Second minimum must be a number");
            return;
        }
        second_max = second_max_box_->text().toDouble(&ok);
        if(!ok){
            QMessageBox::warning(this, "Invalid Input", "Second maximum must be a number");
            return;
        }
        try{
            data_->BandRatio(first_min, first_max, second_min, second_max, name, method);
        }
        catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
        break;
    default:
        try{
            data_->Univariate(first_min, first_max, name, method, integration_method);
        }
        catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }

        break;
    }
}
