#include "GUI/Display/spectrumselectiondialog.h"
#include "ui_spectrumselectiondialog.h"

SpectrumSelectionDialog::SpectrumSelectionDialog(QWidget *parent, MainWindow *main_window, QSharedPointer<VespucciDataset> dataset) :
    QDialog(parent),
    ui(new Ui::SpectrumSelectionDialog)
{
    dataset_ = dataset;
    ui->setupUi(this);
    table_model_ = new SpectraTableModel(this, dataset);
    main_window_ = main_window;
    spectrum_viewer_ = new SpectrumViewer(this, dataset);
    spectrum_viewer_->show();
    table_view_ = findChild<QTableView *>("tableView");
    table_view_->setModel(table_model_);
    workspace = main_window->workspace_ptr();
}

SpectrumSelectionDialog::~SpectrumSelectionDialog()
{
    spectrum_viewer_->close();
    delete ui;
}

void SpectrumSelectionDialog::on_tableView_clicked(const QModelIndex &index)
{
    uword spectrum_index = index.row();
    QVector<double> spectrum = dataset_->PointSpectrum(spectrum_index);
    QVector<double> wavelength = dataset_->WavelengthQVector();
    spectrum_viewer_->SetPlot(wavelength, spectrum);
}

void SpectrumSelectionDialog::SpectrumRemoved(int row)
{
    //after deletion, row may be out of bounds
    if (row >= dataset_->spectra_ptr()->n_cols)
        row = dataset_->spectra_ptr()->n_cols - 1; //make the last row appear.

    try{
        spectrum_viewer_->SetPlot(dataset_->WavelengthQVector(),
                                  dataset_->PointSpectrum(row));
    }
    catch(std::exception e){
        main_window_->DisplayExceptionWarning(e);
    }
}

void SpectrumSelectionDialog::on_pushButton_clicked()
{
    int row = table_view_->currentIndex().row();

    int response = QMessageBox::question(this, "Delete Spectrum?",
                                        "Are you sure you want to delete the spectrum at index " + QString::number(row) + "?",
                                         QMessageBox::Yes, QMessageBox::No);
    cout << (response == QMessageBox::Yes ? "QMessageBox::Yes" : "!QMessageBox::Yes") << endl;
    if (response == QMessageBox::Yes){
        try{
            cout << "call ShedRow" << endl;
            dataset_->ShedSpectrum(row);
        }
        catch(std::exception e){
            main_window_->DisplayExceptionWarning(e);
        }
    }

}

void SpectrumSelectionDialog::on_pushButton_2_clicked()
{
    int row = table_view_->currentIndex().row();
    QString filename =
            QFileDialog::getSaveFileName(this, "Save Spectrum",
                                         workspace->directory(),
                                         "Comma-separated variables (*.csv);; "
                                         "Tab-delimited text (*.txt);; "
                                         "Armadillo binary (*.arma)");
    QFileInfo file_info(filename);
    QString extension = file_info.suffix();
    bool success;
    file_type type;

    if (extension.toLower() == "arma")
        type = arma_binary;
    if (extension.toLower() == "txt")
        type = raw_ascii;
    else
        type = csv_ascii;

    success = dataset_->SaveSpectrum(filename, row, type);
    if (success)
        QMessageBox::information(main_window_, "File Saved", filename + " saved successfully");
    else
        QMessageBox::warning(main_window_, "File not saved", "File not saved successfully");

}
