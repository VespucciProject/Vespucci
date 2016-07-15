#include "GUI/Display/spectrumselectiondialog.h"
#include "ui_spectrumselectiondialog.h"

SpectrumSelectionDialog::SpectrumSelectionDialog(MainWindow *main_window) :
    QDialog(main_window),
    ui(new Ui::SpectrumSelectionDialog)
{
    ui->setupUi(this);
    main_window_ = main_window;
    workspace_ = main_window->workspace_ptr();
    plot_viewer_ = main_window_->plot_viewer();
    dataset_ = QSharedPointer<VespucciDataset>(0);
}

SpectrumSelectionDialog::~SpectrumSelectionDialog()
{
    delete ui;
}

void SpectrumSelectionDialog::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev);
    if (dataset_.data()){
        table_model_ = new SpectraTableModel(this, dataset_);
        ui->tableView->setModel(table_model_);
    }

}

void SpectrumSelectionDialog::closeEvent(QCloseEvent *ev)
{
    QDialog::closeEvent(ev);
    emit SetActionChecked(false);
}

void SpectrumSelectionDialog::DatasetSelectionChanged(QString dataset_key)
{
    if (dataset_->name() != dataset_key)
        dataset_ = workspace_->GetDataset(dataset_key);
    if (isVisible()){
        table_model_ = new SpectraTableModel(this, dataset_);
        ui->tableView->setModel(table_model_);
    }
}

void SpectrumSelectionDialog::DatasetToBeRemoved(QString key)
{
    if (dataset_.data() && dataset_->name() == key){
        table_model_ = new SpectraTableModel(this);
        ui->tableView->setModel(table_model_);
    }

}

void SpectrumSelectionDialog::on_tableView_clicked(const QModelIndex &index)
{
    if (!dataset_.data()) return; //occurs when dialog is open and parent dataset removed
    if (index.isValid() && table_model_->rowCount(index)){
        QSharedPointer<mat> data(new mat(join_horiz(dataset_->abscissa(),
                                                    dataset_->spectra_ptr()->col(index.row()))));
        if (ui->holdCheckBox->isChecked())
            plot_viewer_->AddPlot(*data, dataset_->name());
        else
            plot_viewer_->AddTransientPlot(*data, dataset_->name());
    }

}

void SpectrumSelectionDialog::SpectrumRemoved(int row)
{
    if (!dataset_.data()) return; //should never happen, but let's be safe

    //after deletion, row may be out of bounds
    if (row >= dataset_->spectra_ptr()->n_cols)
        row = dataset_->spectra_ptr()->n_cols - 1; //make the last row appear.

    try{
        uvec indices = {uword(row)};
        if (ui->holdCheckBox->isChecked())
            plot_viewer_->AddPlot(dataset_->abscissa(),
                                  dataset_->spectra(indices),
                                  dataset_->name());

        else
            plot_viewer_->AddTransientPlot(dataset_->abscissa(),
                                           dataset_->spectra(indices),
                                           dataset_->name());


    }
    catch(std::exception e){
        main_window_->DisplayExceptionWarning(e);
    }
}

void SpectrumSelectionDialog::on_deletePushButton_clicked()
{
    if (!dataset_.data()) return; //should never happen, but let's be safe

    int row = ui->tableView->currentIndex().row();

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

void SpectrumSelectionDialog::on_exportPushButton_clicked()
{
    if (!dataset_.data()) return; //should never happen, but let's be safe

    int row = ui->tableView->currentIndex().row();
    QString filename =
            QFileDialog::getSaveFileName(this, "Save Spectrum",
                                         workspace_->directory(),
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
