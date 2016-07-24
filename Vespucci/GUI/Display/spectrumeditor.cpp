#include "GUI/Display/spectrumeditor.h"
#include "ui_SpectrumEditor.h"

SpectrumEditor::SpectrumEditor(MainWindow *main_window, QSharedPointer<VespucciWorkspace> workspace) :
    QDockWidget(main_window),
    ui(new Ui::SpectrumEditor)
{
    ui->setupUi(this);
    main_window_ = main_window;
    workspace_ = workspace;
    plot_viewer_ = main_window_->plot_viewer();
    dataset_ = QSharedPointer<VespucciDataset>(0);
    table_model_ = NULL;
}

SpectrumEditor::~SpectrumEditor()
{
    delete ui;
    if (table_model_) delete table_model_;
}

void SpectrumEditor::closeEvent(QCloseEvent *ev)
{
    QDockWidget::closeEvent(ev);
    emit SetActionChecked(false);
}

void SpectrumEditor::keyPressEvent(QKeyEvent *event)
{
    QModelIndex current_index = ui->tableView->currentIndex();
    if (event->key() == Qt::Key_Enter)
        RequestSpectrumPlot(current_index);
}

void SpectrumEditor::DatasetSelectionChanged(QString dataset_key)
{
    dataset_ = workspace_->GetDataset(dataset_key);
    if (dataset_.isNull()) return;
    if (table_model_) delete table_model_;
    table_model_ = new SpectraTableModel(this, dataset_);
    ui->tableView->setModel(table_model_);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    //we'll change even if not visible, the overhead of creating the model
    //isn't that high.
}

void SpectrumEditor::DatasetToBeRemoved(QString key)
{
    if (!dataset_.isNull() && dataset_->name() == key){
        if (table_model_) delete table_model_;
        table_model_ = new SpectraTableModel(this);
        ui->tableView->setModel(table_model_);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->resizeRowsToContents();
    }

}

void SpectrumEditor::on_tableView_clicked(const QModelIndex &index)
{
    RequestSpectrumPlot(index);
}

void SpectrumEditor::SpectrumRemoved(int row)
{
    if (dataset_.isNull()) return; //should never happen, but let's be safe

    //after deletion, row may be out of bounds
    if (uword(row) >= dataset_->spectra_ptr()->n_cols)
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

void SpectrumEditor::on_deletePushButton_clicked()
{
    if (dataset_.isNull()) return; //should never happen, but let's be safe

    int row = ui->tableView->currentIndex().row();

    int response = QMessageBox::question(this, "Delete Spectrum?",
                                        "Are you sure you want to delete the spectrum at index " + QString::number(row) + "?",
                                         QMessageBox::Yes, QMessageBox::No);
    cout << (response == QMessageBox::Yes ? "QMessageBox::Yes" : "!QMessageBox::Yes") << "\n";

    if (response == QMessageBox::Yes){
        try{
            cout << "call ShedRow\n";

            dataset_->ShedSpectrum(row);
        }
        catch(std::exception e){
            main_window_->DisplayExceptionWarning(e);
        }
    }
}

void SpectrumEditor::on_exportPushButton_clicked()
{
    if (dataset_.isNull()) return; //should never happen, but let's be safe

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

void SpectrumEditor::RequestSpectrumPlot(const QModelIndex &index)
{
    if (!dataset_.isNull() && index.isValid() && table_model_->rowCount(index)){
        QSharedPointer<mat> data(new mat(join_horiz(dataset_->abscissa(),
                                                    dataset_->spectra_ptr()->col(index.row()))));
        if (ui->holdCheckBox->isChecked())
            plot_viewer_->AddPlot(*data, dataset_->name());
        else
            plot_viewer_->AddTransientPlot(*data, dataset_->name());
    }
}
