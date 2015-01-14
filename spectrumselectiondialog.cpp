#include "spectrumselectiondialog.h"
#include "ui_spectrumselectiondialog.h"

SpectrumSelectionDialog::SpectrumSelectionDialog(QWidget *parent, MainWindow *main_window, QSharedPointer<VespucciDataset> dataset) :
    QDialog(parent),
    ui(new Ui::SpectrumSelectionDialog)
{
    dataset_ = dataset;
    cout << "constructor" << endl;
    ui->setupUi(this);
    cout << "table mdoel" << endl;
    table_model_ = new SpectraTableModel(this, dataset);
    main_window_ = main_window;
    cout << "spectrum viewer" << endl;
    spectrum_viewer_ = new SpectrumViewer(this, dataset);
    spectrum_viewer_->show();
    cout << "find table view" << endl;
    table_view_ = this->findChild<QTableView *>("tableView");
    cout << "set model" << endl;
    table_view_->setModel(table_model_);


}

SpectrumSelectionDialog::~SpectrumSelectionDialog()
{
    delete ui;
}

void SpectrumSelectionDialog::on_tableView_clicked(const QModelIndex &index)
{
    uword spectrum_index = index.row();
    cout << "on_tableView_clicked" << endl;

    cout << "call PointSpectrum" << endl;
    QVector<double> spectrum = dataset_->PointSpectrum(spectrum_index);
    cout << "call WavelengthQVector" << endl;
    QVector<double> wavelength = dataset_->WavelengthQVector();
    cout << "call SetPlot" << endl;
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
    cout << "SpectrumSelectionDialog::on_pushButton_clicked()" << endl;
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
