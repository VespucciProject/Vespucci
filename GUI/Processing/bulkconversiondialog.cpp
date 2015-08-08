#include "GUI/Processing/bulkconversiondialog.h"
#include "ui_bulkconversiondialog.h"
#include "Math/VespucciMath.h"
#include "Data/Import/binaryimport.h"
#include "Data/Import/textimport.h"
#include "GUI/Display/reportmessagedialog.h"

BulkConversionDialog::BulkConversionDialog(MainWindow *parent,
                                           VespucciWorkspace *ws) :
    QDialog(parent),
    ui(new Ui::BulkConversionDialog)
{
    ui->setupUi(this);
    workspace = ws;
    filename_list_widget_ = findChild<QListWidget*>("filenameListWidget");
    swap_box_ = findChild<QCheckBox*>("swapCheckBox");
    intype_box_ = findChild<QComboBox*>("inputComboBox");
    outtype_box_ = findChild<QComboBox*>("outputComboBox");
    target_line_edit_ = findChild<QLineEdit*>("targetLineEdit");

    filename_list_widget_->setSelectionMode(QAbstractItemView::MultiSelection);
    //so delete key can be used to remove filenames from list
    QShortcut *shortcut =
            new QShortcut(QKeySequence(Qt::Key_Delete), filename_list_widget_);
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
                                          workspace->directory(),
                                          "Text (*.txt *.csv);;"
                                          "Binary(*.arma *.bin *.dat)");
    filename_list_widget_->addItems(filename_list);
}

void BulkConversionDialog::DeleteItem()
{
    QList<QListWidgetItem*> items = filename_list_widget_->selectedItems();
    foreach(QListWidgetItem *item, items)
        filename_list_widget_->removeItemWidget(item);
}

void BulkConversionDialog::on_buttonBox_accepted()
{
    close();
    using namespace arma;
    filename_list_widget_->selectAll();
    QList<QListWidgetItem *> items = filename_list_widget_->selectedItems();
    vector<string> infile_names;

    foreach(QListWidgetItem * item, items)
        infile_names.push_back(item->text().toStdString());

    string outfile_path = target_line_edit_->text().toStdString();
    infile_type intype;
    arma::file_type outtype;

    switch (intype_box_->currentIndex()){
      case 0:
        intype = wide_text;
        break;
      case 1:
        intype = long_text;
        break;
      case 2: default:
        intype = binary;
    }

    switch (outtype_box_->currentIndex()){
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
    QProgressDialog *progress = new QProgressDialog("Loading file", "Cancel", 0, 100);
    switch (intype){
      case wide_text:
        for(strvec_it it = infile_names.begin(); it!=infile_names.end(); ++it){
            QString sep = GetSep(QString::fromStdString(*it));
            bool valid = TextImport::CheckFileValidity(QString::fromStdString(*it), comma_decimals);

            if (valid){
                ok = TextImport::ImportWideText(QString::fromStdString(*it),
                                                spectra,
                                                abscissa,
                                                x, y,
                                                swap_spatial,
                                                progress, sep);
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
        progress->close();
        break;

      case long_text:
        for(strvec_it it = infile_names.begin(); it!=infile_names.end(); ++it){
            valid = TextImport::CheckFileValidity(QString::fromStdString(*it), comma_decimals);

            if (valid){
                ok = TextImport::ImportLongText(QString::fromStdString(*it),
                                                spectra,
                                                abscissa,
                                                x, y,
                                                swap_spatial,
                                                progress);
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
        progress->close();
        break;

      case binary: default:
        for(strvec_it it = infile_names.begin(); it!=infile_names.end(); ++it){
            ok = BinaryImport::ImportVespucciBinary(QString::fromStdString(*it),
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
    case arma_binary: default:
        Vespucci::SaveVespucciBinary(filename + ".vds", spectra, x, y, abscissa);
    }
}

void BulkConversionDialog::on_BrowsePushButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select Folder",
                                                     workspace->directory());
    target_line_edit_->setText(path);
}
