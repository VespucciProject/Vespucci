#include "stitchimportdialog.h"
#include "ui_stitchimportdialog.h"
#include <Data/Import/binaryimport.h>
#include "Data/Import/textimport.h"

StitchImportDialog::StitchImportDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::StitchImportDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
}

StitchImportDialog::~StitchImportDialog()
{
    delete ui;
}

void StitchImportDialog::on_browsePushButton_clicked()
{
    QString filename =
            QFileDialog::getOpenFileName(this,
                                         "Select Instruction File",
                                         workspace_->directory(),
                                         "Instruction File (*.csv)");
    ui->filenameLineEdit->setText(filename);
}

bool StitchImportDialog::LoadDatasets(field<string> filenames, mat &spectra, vec &x, vec &y, vec &abscissa, bool swap_spatial, QString type)
{
    //follows the vespuccci binary field format:
    field<field<mat> > datasets(filenames.n_rows, filenames.n_cols);
    mat current_spectra;
    vec current_x, current_y, current_abscissa;
    bool ok;
    bool two_dim = datasets.n_rows > 1 && datasets.n_cols > 1;
    for (uword j = 0; j < filenames.n_cols; ++j){
        for (uword i = 0; i < filenames.n_rows; ++i){
            QString filename;
            if (two_dim)
                filename = path_ + "/" + QString::fromStdString(filenames(i, j));
            else
                filename = path_ + "/" + QString::fromStdString(filenames(i));
            if (type == "Vespucci Dataset"){

                ok = BinaryImport::ImportVespucciBinary(filename.toStdString(),
                                                        current_spectra,
                                                        current_abscissa,
                                                        current_x,
                                                        current_y);
            }
            else if (type == "Wide Text" || type == "Wide CSV"){
                ok = TextImport::ImportWideText(filename.toStdString(),
                                                current_spectra,
                                                current_abscissa,
                                                current_x,
                                                current_y,
                                                swap_spatial);


            }

            else if (type == "Long Text" || "Long CSV"){
                ok = TextImport::ImportLongText(filename.toStdString(),
                                                current_spectra,
                                                current_abscissa,
                                                current_x,
                                                current_y,
                                                swap_spatial);
            }
            else{
                cout << "Improper!\n";

                return false;
            }



            if (ok){
                field<mat> dataset(4);
                dataset(0) = current_spectra;
                dataset(1) = current_abscissa;
                dataset(2) = current_x;
                dataset(3) = current_y;
                datasets(i, j) = dataset;
            }
        }
    }

    try{
        ok = Vespucci::StitchDatasets(datasets, spectra, x, y, abscissa);
    }catch(exception e){
        workspace_->main_window()->DisplayExceptionWarning("Vespucci::StitchDatasets", e);
        ok = false;
    }

    return ok;
}

void StitchImportDialog::on_buttonBox_accepted()
{
    QString filename = ui->filenameLineEdit->text();
    path_ = QFileInfo(filename).absolutePath();
    QString data_format = ui->formatComboBox->currentText();
    bool swap_spatial = ui->swapSpatialCheckBox->isChecked();
    bool ok;
    mat spectra;
    vec x, y, abscissa;
    field<string> instruction;
    ok = instruction.load(filename.toStdString());
    if (!ok){
        QMessageBox::warning(this,
                             "Failed to Load Instructions",
                             "The instruction file could not be loaded");
    }
    try{
        ok = LoadDatasets(instruction,
                          spectra, x, y, abscissa,
                          swap_spatial, data_format);
    }catch (exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
        ok = false;
    }
    if (!ok){
        QMessageBox::warning(this,
                             "Failed to Load Dataset",
                             "One or more datasets could not be loaded, or "
                             "datasets of incompatible spatial coordinates.");
    }
    else{
        QString x_description = ui->xLineEdit->text();
        QString x_units = ui->xUnitsLineEdit->text();
        QString y_description = ui->yLineEdit->text();
        QString y_units = ui->yUnitsLineEdit->text();
        QString name = ui->nameLineEdit->text();
        QSharedPointer<VespucciDataset>
                dataset(new VespucciDataset(name,
                                            workspace_->main_window(),
                                            workspace_));
        dataset->SetData(spectra, abscissa, x, y);
        dataset->SetXDescription(x_description + " (" + x_units + ")");
        dataset->SetYDescription(y_description + " (" + y_units + ")");
        workspace_->AddDataset(dataset);
    }
}
