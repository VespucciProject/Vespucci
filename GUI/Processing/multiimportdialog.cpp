#include "GUI/Processing/multiimportdialog.h"
#include "ui_multiimportdialog.h"
#include "Data/Import/textimport.h"

MultiImportDialog::MultiImportDialog(QWidget *parent, VespucciWorkspace *ws) :
    QDialog(parent),
    ui(new Ui::MultiImportDialog)
{
    ui->setupUi(this);
    workspace = ws;
    filename_table_ = findChild<QTableWidget*>("filenameTableWidget");
    count_label_ = findChild<QLabel*>("countLabel");
    abscissa_label_box_ = findChild<QLineEdit*>("abscissaLineEdit");
    ordinate_label_box_ = findChild<QLineEdit*>("ordinateLineEdit");
    name_box_ = findChild<QLineEdit*>("nameLineEdit");
    ws->dataset_loading_count();
    name_box_->setText("Dataset" + QString::number(ws->dataset_loading_count()));

    filename_table_->setRowCount(1);
    filename_table_->setColumnCount(1);
}

MultiImportDialog::~MultiImportDialog()
{
    delete ui;
}

void MultiImportDialog::on_rowSpinBox_valueChanged(int arg1)
{
    filename_table_->setRowCount(arg1);
    count_label_->setText(QString::number(arg1 * filename_table_->columnCount()));
}

void MultiImportDialog::on_colSpinBox_valueChanged(int arg1)
{
    filename_table_->setColumnCount(arg1);
    count_label_->setText(QString::number(arg1 * filename_table_->rowCount()));
}

void MultiImportDialog::on_addFilesPushButton_clicked()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this, "Select Files", workspace->directory(), "Text Files (*.txt *.csv *.dat);;"
                                                                                                        "All Files");
    QStringList filenames_cpy = filenames;
    filenames_cpy.sort();
    QString path = filenames_cpy.first();
    QFileInfo file_info(path);
    QString new_directory = file_info.absoluteDir().absolutePath();
    workspace->set_directory(new_directory);

    QStringList::Iterator it = filenames_cpy.begin();
    int current_row = filename_table_->currentRow();
    int current_col = filename_table_->currentColumn();
    int col_count = filename_table_->columnCount();
    int row_count = filename_table_->rowCount();
    //add selected file names to the table in alphabetical order starting with
    //the currently highlighted cell
    while (current_row < row_count){
        //iterate over columns in this row
        while (current_col < col_count && it != filenames_cpy.end()){
            //add approprate widget item containing the filename to the
            QTableWidgetItem *current_item = new QTableWidgetItem;
            current_item->setText(*it);
            filename_table_->setItem(current_row, current_col, current_item);
            ++it;
            ++current_col;
        }
        ++current_row;
        current_col = 0;
    }
    if (it != filenames_cpy.end()){
        QMessageBox::warning(this,
                             "Too many files selected.",
                             "The number of files selected exceeds the number of"
                             "cells in the table. Not all filenames have been "
                             "added to the table.");
    }


}



void MultiImportDialog::on_buttonBox_accepted()
{
    int cols = filename_table_->columnCount();
    int rows = filename_table_->rowCount();
    QString name = name_box_->text();
    QString x_axis_description = abscissa_label_box_->text();
    QString y_axis_description = ordinate_label_box_->text();

    QMap<QPair<int, int>, QString> filename_map;

    for (int row = 0; row < rows; ++row){
        for (int col = 0; col < cols; ++col){
            QString filename = filename_table_->itemAt(row, col)->text();
            filename_map.insert(QPair<int, int>(row, col), filename);
        }
    }

    arma::mat spectra;
    arma::vec x, y, abscissa;
    bool ok = TextImport::ImportMultiplePoints(filename_map,
                                               rows, cols,
                                               spectra,
                                               abscissa,
                                               x, y);
    if (!ok)
        QMessageBox::warning(this, "Import Failure", "Import of at least one file failed, or no abscissa present in any file.");

    else{
        QSharedPointer<VespucciDataset> dataset(new VespucciDataset(filename_map,
                                                                    workspace->main_window(),
                                                                    workspace->directory_ptr(),
                                                                    workspace->CreateLogFile(name),
                                                                    name,
                                                                    x_axis_description,
                                                                    y_axis_description,
                                                                    rows, cols));
        workspace->AddDataset(dataset);
    }

}
