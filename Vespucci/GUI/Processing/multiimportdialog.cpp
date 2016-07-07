#include "GUI/Processing/multiimportdialog.h"
#include "ui_multiimportdialog.h"
#include "Data/Import/textimport.h"

MultiImportDialog::MultiImportDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::MultiImportDialog)
{
    ui->setupUi(this);
    workspace_ = ws;
    ws->dataset_loading_count();
    ui->nameLineEdit->setText("Dataset" + QString::number(ws->dataset_loading_count()));

    ui->filenameTableWidget->setRowCount(1);
    ui->filenameTableWidget->setColumnCount(1);
}

MultiImportDialog::~MultiImportDialog()
{
    delete ui;
}

void MultiImportDialog::on_rowSpinBox_valueChanged(int arg1)
{
    ui->filenameTableWidget->setRowCount(arg1);
    ui->countLabel->setText(QString::number(arg1 * ui->filenameTableWidget->columnCount()));
}

void MultiImportDialog::on_colSpinBox_valueChanged(int arg1)
{
    ui->filenameTableWidget->setColumnCount(arg1);
    ui->countLabel->setText(QString::number(arg1 * ui->filenameTableWidget->rowCount()));
}

void MultiImportDialog::on_addFilesPushButton_clicked()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this, "Select Files", workspace_->directory(), "Text Files (*.txt *.csv *.dat);;"
                                                                                                        "All Files");
    QStringList filenames_cpy = filenames;
    filenames_cpy.sort();
    QString path = filenames_cpy.first();
    QFileInfo file_info(path);
    QString new_directory = file_info.absoluteDir().absolutePath();
    workspace_->set_directory(new_directory);

    QStringList::Iterator it = filenames_cpy.begin();
    int current_row = ui->filenameTableWidget->currentRow();
    int current_col = ui->filenameTableWidget->currentColumn();
    int col_count = ui->filenameTableWidget->columnCount();
    int row_count = ui->filenameTableWidget->rowCount();
    //add selected file names to the table in alphabetical order starting with
    //the currently highlighted cell
    while (current_row < row_count){
        //iterate over columns in this row
        while (current_col < col_count && it != filenames_cpy.end()){
            //add approprate widget item containing the filename to the
            QTableWidgetItem *current_item = new QTableWidgetItem;
            current_item->setText(*it);
            ui->filenameTableWidget->setItem(current_row, current_col, current_item);
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
    int cols = ui->filenameTableWidget->columnCount();
    int rows = ui->filenameTableWidget->rowCount();
    QString name = ui->nameLineEdit->text();
    QString x_axis_description = ui->abscissaLineEdit->text();
    QString y_axis_description = ui->ordinateLineEdit->text();

    map<pair<int,int>, string> filename_map;

    for (int row = 0; row < rows; ++row){
        for (int col = 0; col < cols; ++col){
            QTableWidgetItem *item = ui->filenameTableWidget->takeItem(row, col);
            string value = item->text().toStdString();
            pair<int, int> key(row, col);
            filename_map[key] = value;
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
                                                                    workspace_->main_window(),
                                                                    workspace_->directory_ptr(),
                                                                    workspace_->CreateLogFile(name),
                                                                    name,
                                                                    x_axis_description,
                                                                    y_axis_description,
                                                                    rows, cols));
        workspace_->AddDataset(dataset);
    }

}
