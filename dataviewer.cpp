#include "dataviewer.h"
#include "ui_dataviewer.h"

DataViewer::DataViewer(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::DataViewer)
{
    cout << "DataViewer::DataViewer" << endl;
    ui->setupUi(this);
    workspace = ws;
    cout << "get dataset" << endl;
    dataset_ = workspace->DatasetAt(row);
    cout << "find children" << endl;
    data_selector_ = this->findChild<QComboBox *>("comboBox");
    cout << "test data_selector" << endl;
    cout << data_selector_->font().family().toStdString() << endl;
    table_ = this->findChild<QTableWidget *>("tableWidget");

    cout << "populate list" << endl;
    QStringList object_list("Spectra Matrix");
    object_list << "Spectral Abcissa" << "Spatial Data"
                << "All Data";

    if (dataset_->principal_components_calculated()){
        object_list << "PCA Coefficents" << "PCA Projected Data"
                    << "Eigenvalues of Covariance Matrix" << "PCA t² Values";
    }
    if (dataset_->vertex_components_calculated()){
        object_list << "VCA Coefficients" << "VCA Projected Data"
                    << "VCA Endmembers" << "VCA Pure Components";
    }
    if (dataset_->partial_least_squares_calculated()){
        object_list << "PLS Variance" << "PLS Predictor Loading"
                    << "PLS Response Loading";
    }

    cout << "assign list" << endl;
    constructor_in_progress_ = true;
    data_selector_->addItems(object_list);
    constructor_in_progress_ = false;
    data_selector_->setCurrentIndex(0);
    cout << "end of constructor";
}

DataViewer::~DataViewer()
{
    delete ui;
}

void DataViewer::on_comboBox_currentTextChanged(const QString &arg1)
{
    cout << "DataViewer::on_comboBox_currentTextChanged" << endl;
    cout << "another line" << endl;
    if (constructor_in_progress_){
        cout << "constructor in progress" << endl;
        return;
    }
    if (arg1 == NULL){
        cout << "arg1 is NULL";
        return;
    }
    cout << arg1.toStdString();
    unsigned int i, j;
    if (arg1 == "Spectra Matrix"){
        cout << "Spectra Matrix" << endl;
        mat spectra = dataset_->spectra();
        table_->clear();
        const unsigned int rows = spectra.n_rows;
        const unsigned int cols = spectra.n_cols;
        table_->setRowCount(rows);
        table_->setColumnCount(cols);
        QString value;
        for (i = 0; i < rows; ++i){
            for (j = 0; j < cols; ++j){
                value = QString::number(spectra(i, j));
                cout << value.toStdString() << endl;
                table_->setItem(i, j, new QTableWidgetItem(value, 0));
            }
        }
    }
    else if (arg1 == "Spatial Data"){
        cout << "Spatial Data";

        colvec x = dataset_->x();
        colvec y = dataset_->y();
        table_->clear();
        const unsigned int rows = x.n_rows;
        table_->setRowCount(rows);
        table_->setColumnCount(2);
        for (i = 0; i < rows; ++i){
            table_->setItem(i, 0,
                            new QTableWidgetItem
                            (QString::number(x(i)), 0));
            table_->setItem(i, 1,
                            new QTableWidgetItem
                            (QString::number(y(i)), 0));
        }
    }
    else if (arg1 == "Spectral Abcissa"){
        cout << "Spectral Abcissa";
        rowvec wavelength = dataset_->wavelength();
        table_->clear();
        const unsigned int cols = wavelength.n_cols;
        table_->setRowCount(1);
        table_->setColumnCount(cols);
        for (j = 0; j < cols; ++j){
            table_->setItem(0, j,
                            new QTableWidgetItem
                            (QString::number(wavelength(j)), 0));
        }
    }

    else if (arg1 == "All Data"){
        cout << "All Data";
        mat spectra = dataset_->spectra();
        rowvec wavelength = dataset_->wavelength();
        colvec x = dataset_->x();
        colvec y = dataset_->y();

        const unsigned int rows = x.n_rows + 1;
        const unsigned int cols = wavelength.n_cols + 2;

        table_->clear();
        table_->setRowCount(rows);
        table_->setColumnCount(cols);

        for (j = 2; j < cols; ++j){
            table_->setItem(0, j,
                            new QTableWidgetItem
                            (QString::number(wavelength(j-2)), 0));
        }

        for (i = 1; i < rows; ++i){
            table_->setItem(i, 0,
                            new QTableWidgetItem
                            (QString::number(x(i)), 0));
            table_->setItem(i, 1,
                            new QTableWidgetItem
                            (QString::number(y(i)), 0));
            for (j = 2; j < cols; ++j){
                table_->setItem(i, j,
                                new QTableWidgetItem
                                (QString::number(spectra(i-1, j-2)), 0));
            }
        }
    }

    else if (arg1 == "PCA Coefficients"){
        mat coefficients = *dataset_->principal_components_data()->coeff();
        table_->clear();
        const unsigned int rows = coefficients.n_rows;
        const unsigned int cols = coefficients.n_cols;
        table_->setRowCount(rows);
        table_->setColumnCount(cols);
        for (i = 0; i < rows; ++i){
            for (j = 0; j < cols; ++j){
                table_->setItem(i, j,
                                new QTableWidgetItem
                                (QString::number(coefficients(i, j)), 0));
            }
        }
    }
    else if (arg1 == "PCA Projected Data"){
        mat score = *dataset_->principal_components_data()->score();
        table_->clear();
        const unsigned int rows = score.n_rows;
        const unsigned int cols = score.n_cols;
        table_->setRowCount(rows);
        table_->setColumnCount(cols);
        for (i = 0; i < rows; ++i){
            for (j = 0; j < cols; ++j){
                table_->setItem(i, j,
                                new QTableWidgetItem
                                (QString::number(score(i, j)), 0));
            }
        }
    }

    else if (arg1 == "Eigenvalues of Covariance Matrix"){
        vec latent = *dataset_->principal_components_data()->latent();
        table_->clear();
        const unsigned int rows = latent.n_rows;
        table_->setRowCount(rows);
        table_->setColumnCount(1);
        for (i = 0; i < rows; ++i){
            table_->setItem(i, 0,
                            new QTableWidgetItem
                                (QString::number(latent(i)), 0));
        }
    }
    else if (arg1 == "PCA t² Values"){
        vec tsquared = *dataset_->principal_components_data()->tsquared();
        table_->clear();
        const unsigned int rows = tsquared.n_rows;
        table_->setRowCount(rows);
        table_->setColumnCount(1);
        for (i = 0; i < rows; ++i){
            table_->setItem(i, 0,
                            new QTableWidgetItem
                                (QString::number(tsquared(i)), 0));
        }
    }
    else{
        cout << "Else" << endl;
        table_->clear();
        table_->setRowCount(1);
        table_->setColumnCount(1);
        table_->setItem(0, 0, new QTableWidgetItem("No object selected"));
        table_->resizeColumnsToContents();
        table_->resizeRowsToContents();
    }
    //table_->resizeColumnsToContents();
    //table_->resizeRowsToContents();
}
