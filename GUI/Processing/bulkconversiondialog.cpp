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
    using namespace arma;
    filename_list_widget_->selectAll();
    QList<QListWidgetItem *> items = filename_list_widget_->selectedItems();
    vector<string> infile_names;

    foreach(QListWidgetItem * item, items)
        infile_names.push_back(item->text().toStdString());

    string outfile_path = target_line_edit_->text().toStdString();
    file_type type;
    infile_type intype;
    outfile_type outtype;

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
        outtype = v_binary;
        type = arma_binary;
        break;
    case 1:
        outype = z_csv;
        type = csv_ascii;
        break;
    case 2: default:
        outtype = z_txt;
        type = raw_ascii;
    }
    vector<string> skipped = SaveFiles(infile_names, outfile_path, intype, outtype);
    if(skipped.size()){
        ReportMessageDialog *report_dialog = new ReportMessageDialog(this, "Files Not Imported");
        report_dialog->setLabel("Import or export failed for the following files:");
        for (auto& skipped_name : skipped)
            report_dialog->appendPlainText(QString(skipped_name));
        report_dialog->show();
    }
}

vector<string> BulkConversionDialog::SaveFiles(vector<string> infile_names, string outfile_path, BulkConversionDialog::infile_type intype, BulkConversionDialog::outfile_type outtype, bool swap_spatial) const
{

    bool valid, ok;
    vector<string> skipped_files;

    typedef vector<string>::iterator strvec_it;
    mat spectra;
    vec x, y, abscissa;
    QProgressDialog *progress = new QProgressDialog("Loading file", "Cancel", 0, 100, this);

    switch (intype){
      case wide_text:
        for(strvec_it it = infile_names.begin(); it!=infile_names.end(); ++it){
            QString sep = GetSep(QString(*it));
            bool valid = CheckFileValidity(filename, comma_decimals);

            if (valid){
                ok = TextImport::ImportWideText(QString(*it),
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
                    QFileInfo(QString(*it)).fileName().toStdString();
            WriteFile(outfile_type, outfilename, spectra, abscissa, x, y);
        }//for (infile)

      case long_text:
        for(strvec_it it = infile_names.begin(); it!=infile_names.end(); ++it){
            valid = CheckFileValidity(filename, comma_decimals);

            if (valid){
                ok = TextImport::ImportLongText(QString(infile_name),
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
                    QFileInfo(QString(*it)).fileName().toStdString();

            WriteFile(outfile_type, outfilename, spectra, abscissa, x, y);
        }//for (infiles)

      case binary: default:
        for(strvec_it it = infile_names.begin(); it!=infile_names.end(); ++it){
            ok = BinaryImport::ImportVespucciBinary(QString(infile_name),
                                               spectra,
                                               abscissa,
                                               x, y);
            if (!ok){
                skipped_files.push_back(*it);
                infile_names.erase(it);
            }

            string outfilename =
                    outfile_path + "/" +
                    QFileInfo(QString(*it)).fileName().toStdString();
            WriteFile(outfile_type, outfilename, spectra, abscissa, x, y);
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

    bool has_tab = -1 != inputfile.indexOf("\t");
    bool has_comma = -1 != inputfile.indexOf(",");
    bool has_space = -1 !=inputfile.indexOf(" ");

    QString out;
    //check for tabs first, then spaces if no ta
    if(has_tab){return "\t";}
    else if(has_space){return " ";}
    else if(has_comma) {return ",";}
    else {return "";}

}

void BulkConversionDialog::WriteFile(const BulkConversionDialog::outfile_type &type,
                                     const string filename,
                                     const arma::mat &spectra,
                                     const arma::vec &abscissa,
                                     const arma::vec &x,
                                     const arma::vec &y) const
{
    switch(type){
    case z_csv:
        Vespucci::SaveZipped(spectra, abscissa, x, y, filename, arma::csv_ascii);
        break;
    case z_txt:
        Vespucci::SaveZipped(spectra, abscissa, x, y, filename, arma::raw_ascii);
        break;
    case v_binary: default:
        Vespucci::SaveVespucciBinary(spectra, x, y, abscissa);
    }
}
