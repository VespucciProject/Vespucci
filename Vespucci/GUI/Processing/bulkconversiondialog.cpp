#include "GUI/Processing/bulkconversiondialog.h"
#include "ui_bulkconversiondialog.h"
#include "Math/VespucciMath.h"
#include "Data/Import/binaryimport.h"
#include "Data/Import/textimport.h"
#include "GUI/Display/reportmessagedialog.h"


BulkConversionDialog::BulkConversionDialog(MainWindow *parent,
                                           QSharedPointer<VespucciWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::BulkConversionDialog)
{
    ui->setupUi(this);
    workspace_ = ws;

    ui->filenameListWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    //so delete key can be used to remove filenames from list
    QShortcut *shortcut =
            new QShortcut(QKeySequence(Qt::Key_Delete), ui->filenameListWidget);
    connect(shortcut, SIGNAL(activated()), this, SLOT(DeleteItem()));

}

BulkConversionDialog::~BulkConversionDialog()
{
    delete ui;
}

void BulkConversionDialog::on_browsePushButton_clicked()
{
    QStringList filename_list =
            QFileDialog::getOpenFileNames(this,
                                          "Select files",
                                          workspace_->directory(),
                                          "Text (*.txt *.csv);;"
                                          "Binary(*.arma *.bin *.dat)");
    ui->filenameListWidget->addItems(filename_list);
}

void BulkConversionDialog::DeleteItem()
{
    QList<QListWidgetItem*> items = ui->filenameListWidget->selectedItems();
    for (auto item: items)
        ui->filenameListWidget->removeItemWidget(item);
}

void BulkConversionDialog::on_buttonBox_accepted()
{
    close();
    using namespace arma;
    ui->filenameListWidget->selectAll();
    QList<QListWidgetItem *> items = ui->filenameListWidget->selectedItems();
    vector<string> infile_names;

    for (auto item: items)
        infile_names.push_back(item->text().toStdString());

    string outfile_path = ui->targetLineEdit->text().toStdString();
    infile_type intype;
    arma::file_type outtype;

    switch (ui->inputComboBox->currentIndex()){
      case 0:
        intype = wide_text;
        break;
      case 1:
        intype = long_text;
        break;
      case 2:
        intype = binary;
      case 3: default:
        intype = oldbinary;
    }

    switch (ui->outputComboBox->currentIndex()){
    case 0:
        outtype = arma::arma_binary;
        break;
    case 1:
        outtype = arma::csv_ascii;
        break;
    case 2: default:
        outtype = arma::raw_ascii;
    }
    vector<string> skipped = SaveFiles(infile_names, outfile_path, intype, outtype);
    if(skipped.size()){
        ReportMessageDialog *report_dialog = new ReportMessageDialog(this, "Files Not Imported");
        report_dialog->setLabel("Import or export failed for the following files:");
        for (auto& skipped_name : skipped)
            report_dialog->appendPlainText(QString::fromStdString(skipped_name));
        report_dialog->show();
    }
}

vector<string> BulkConversionDialog::SaveFiles(vector<string> infile_names, string outfile_path, BulkConversionDialog::infile_type intype, arma::file_type outtype, bool swap_spatial) const
{

    bool valid, ok;
    vector<string> skipped_files;

    typedef vector<string>::iterator strvec_it;
    mat spectra;
    vec x, y, abscissa;
    bool comma_decimals = false;
    switch (intype){
      case wide_text:
        for(strvec_it it = infile_names.begin(); it!=infile_names.end(); ++it){
            bool valid = TextImport::CheckFileValidity(*it, comma_decimals);

            if (valid){
                ok = TextImport::ImportWideText(*it,
                                                spectra,
                                                abscissa,
                                                x, y,
                                                swap_spatial);
            }

            if (!valid || !ok){
                skipped_files.push_back(*it);
                infile_names.erase(it);
            }

            string outfilename =
                    outfile_path + "/" +
                    QFileInfo(QString::fromStdString(*it)).baseName().toStdString();
            WriteFile(outtype, outfilename, spectra, abscissa, x, y);
        }//for (infile)
        break;

      case long_text:
        for(strvec_it it = infile_names.begin(); it!=infile_names.end(); ++it){
            valid = TextImport::CheckFileValidity(QString::fromStdString(*it), comma_decimals);

            if (valid){
                ok = TextImport::ImportLongText(*it,
                                                spectra,
                                                abscissa,
                                                x, y,
                                                swap_spatial);
            }

            if (!valid || !ok){
                skipped_files.push_back(*it);
                infile_names.erase(it);
            }

            string outfilename =
                    outfile_path + "/" +
                    QFileInfo(QString::fromStdString(*it)).baseName().toStdString();

            WriteFile(outtype, outfilename, spectra, abscissa, x, y);
        }//for (infiles)
        break;
      case binary:
        for (strvec_it it = infile_names.begin(); it != infile_names.end(); ++it){
            QSharedPointer<VespucciDataset> dataset(new VespucciDataset(QString::fromStdString(*it), workspace_->main_window(), workspace_));
            ok = dataset->ConstructorCancelled();
            if (!ok){
                skipped_files.push_back(*it);
                infile_names.erase(it);
            }

            string outfilename =
                    outfile_path + "/" +
                    QFileInfo(QString::fromStdString(*it)).fileName().toStdString();
            WriteFile(outtype, outfilename, dataset->spectra(), dataset->abscissa(), dataset->x(), dataset->y());
      }
      case oldbinary: default:
        for(strvec_it it = infile_names.begin(); it!=infile_names.end(); ++it){
            ok = BinaryImport::ImportVespucciBinary(*it,
                                                    spectra,
                                                    abscissa,
                                                    x, y);
            if (!ok){
                skipped_files.push_back(*it);
                infile_names.erase(it);
            }

            string outfilename =
                    outfile_path + "/" +
                    QFileInfo(QString::fromStdString(*it)).fileName().toStdString();
            WriteFile(outtype, outfilename, spectra, abscissa, x, y);
        }//for (infiles)
    }//switch (intype)
    return skipped_files;
}

const QString BulkConversionDialog::GetSep(const QString &filename) const
{
    QFile inputfile(filename);
    inputfile.open(QIODevice::ReadOnly);
    QTextStream inputstream(&inputfile);
    QString line = inputstream.readLine();
    inputfile.close();


    bool has_tab = -1 != line.indexOf("\t");
    bool has_comma = -1 != line.indexOf(",");
    bool has_space = -1 !=line.indexOf(" ");

    QString out;
    //check for tabs first, then spaces if no ta
    if(has_tab){return "\t";}
    else if(has_space){return " ";}
    else if(has_comma) {return ",";}
    else {return "";}

}

void BulkConversionDialog::WriteFile(const arma::file_type &type,
                                     const string filename,
                                     const arma::mat &spectra,
                                     const arma::vec &abscissa,
                                     const arma::vec &x,
                                     const arma::vec &y) const
{
    switch(type){
    case csv_ascii: case raw_ascii:
        Vespucci::SaveText(filename, spectra, x, y, abscissa, type);
        break;
    case hdf5_binary:
        SaveHDF5(filename, spectra, abscissa, x, y);
    case arma_binary: default:
        Vespucci::SaveOldVespucciBinary(filename + ".vds", spectra, x, y, abscissa);
    }
}

void BulkConversionDialog::on_BrowsePushButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select Folder",
                                                     workspace_->directory());
    ui->targetLineEdit->setText(path);
}

bool BulkConversionDialog::SaveHDF5(string filename,
                                    const mat &spectra,
                                    const vec &abscissa,
                                    const vec &x,
                                    const vec &y) const
{
    VespucciDataset dataset(QString::fromStdString(filename),
                           workspace_->main_window(),
                           workspace_->directory_ptr());
    dataset.SetData(spectra, abscissa, x, y);
    return dataset.Save(QString::fromStdString(filename));
}
