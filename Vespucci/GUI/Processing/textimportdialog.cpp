#include "textimportdialog.h"
#include "ui_textimportdialog.h"
#include <QFileDialog>
#include <QFileInfo>
#include "Data/Import/textimport.h"

TextImportDialog::TextImportDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::TextImportDialog),
    workspace_(ws)
{
    ui->setupUi(this);

}

TextImportDialog::~TextImportDialog()
{
    delete ui;
}

void TextImportDialog::on_browseButton_clicked()
{
    QStringList filenames_ = QFileDialog::getOpenFileNames(this, "Select input files", workspace_->directory());
    if (filenames_.size()){
        ui->filenameListWidget->clear();
        ui->filenameListWidget->addItems(filenames_);
        QFileInfo fi(filenames_.first());
        QString name = fi.baseName();
        ui->nameLineEdit->setText(name);
    }
}

void TextImportDialog::on_buttonBox_accepted()
{
    arma::mat spectra;
    arma::mat temp_spectra;
    arma::vec abscissa;
    bool transpose = ui->transposeCheckBox->isChecked();
    bool plural = ui->pluralCheckBox->isChecked();
    bool ok = true;
    arma::uword start_row = ui->startRowSpinBox->value() - 1;
    arma::uword abs_col = ui->abscissaColumnSpinBox->value() - 1;
    arma::uword spec_col = ui->spectraColumnSpinBox->value() - 1;
    QString name = ui->nameLineEdit->text();
    for (auto filename : filenames_){
        ok = TextImport::ImportText(filename.toStdString(), temp_spectra, abscissa, start_row, abs_col, spec_col, transpose, plural);
        if (ok){
            try {
                if (!spectra.n_elem)
                    spectra = temp_spectra;
                else
                    spectra = arma::join_horiz(spectra, temp_spectra);
            }catch(...){
                ok = false;
            }
        }
    }
    if (ok){
        arma::vec x(spectra.n_cols);
        x.fill(arma::datum::nan);
        arma::vec y = x;
        QSharedPointer<VespucciDataset> dataset(new VespucciDataset(name, workspace_->main_window(), workspace_));
        dataset->SetData(spectra, abscissa, x, y);
        dataset->SetName(name); //double check that SetData doesn't hurt name...
        workspace_->AddDataset(dataset);
    }
    else{
        QMessageBox::warning(this, "An error occurred", "Input is not valid");
    }
    close();
}
