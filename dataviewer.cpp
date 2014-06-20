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
    table_ = this->findChild<QTableView *>("tableView");

    cout << "populate list" << endl;
    QStringList object_list("Spectral Abcissa");
    object_list << "Spatial Data";

    if (dataset_->principal_components_calculated()){
        object_list << "PCA Coefficents" << "Eigenvalues of Covariance Matrix"
                    << "PCA t² Values";
    }
    if (dataset_->vertex_components_calculated()){
        object_list << "VCA Coefficients" << "VCA Endmembers" << "VCA Pure Components";
    }
    if (dataset_->partial_least_squares_calculated()){
        object_list << "PLS Variance" << "PLS Predictor Loading"
                    << "PLS Response Loading";
    }

    cout << "assign list" << endl;
    data_selector_->addItems(object_list);
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
    cout << arg1.toStdString();
    if (arg1 == "Spatial Data"){
        cout << "Spatial Data";
        table_->setModel(new VespucciTableModel(this, dataset_, "spatial"));
    }
    else if (arg1 == "Spectral Abcissa"){
        cout << "Spectral Abcissa";
        table_->setModel(new VespucciTableModel(this, dataset_->wavelength_ptr()));
    }
    else if (arg1 == "PCA Coefficients"){
        mat *coefficients = dataset_->principal_components_data()->coeff();
        table_->setModel(new VespucciTableModel(this, coefficients));
    }
    else if (arg1 == "Eigenvalues of Covariance Matrix"){
        mat *latent = (mat*) dataset_->principal_components_data()->latent();
        table_->setModel(new VespucciTableModel(this, latent));
    }
    else if (arg1 == "PCA t² Values"){
        mat *tsquared = (mat*) dataset_->principal_components_data()->tsquared();
        table_->setModel(new VespucciTableModel(this, tsquared));
    }

    else{
        cout << "Else" << endl;
        table_->setModel(0);
    }

    table_->resizeColumnsToContents();
    table_->resizeRowsToContents();
}
