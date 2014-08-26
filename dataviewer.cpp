#include "dataviewer.h"
#include "ui_dataviewer.h"
#include "spectrumviewer.h"

DataViewer::DataViewer(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::DataViewer)
{
    ui->setupUi(this);
    workspace = ws;
    dataset_ = workspace->DatasetAt(row);
    directory_ = workspace->directory();
    export_button_ = this->findChild<QPushButton *>("exportPushButton");
    plot_button_ = this->findChild<QPushButton *>("plotPushButton");
    data_selector_ = this->findChild<QComboBox *>("comboBox");
    plot_button_->setDisabled(true);
    export_button_->setDisabled(true);
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
                    << "PLS Response Loading" << "PLS Predictor Scores"
                    << "PLS Response Scores" << "PLS Coefficients";
    }
    if (dataset_->k_means_calculated()){
        object_list << "K-means Assignments";
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
    current_text_ = arg1;
    cout << "DataViewer::on_comboBox_currentTextChanged" << endl;
    cout << arg1.toStdString();
    if (arg1 == "Spatial Data"){
        table_->setModel(new VespucciTableModel(this, dataset_, "spatial"));
        export_button_->setDisabled(true);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "Spectral Abcissa"){
        cout << "Spectral Abcissa";
        table_->setModel(new VespucciTableModel(this, dataset_->wavelength_ptr()));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PCA Coefficients"){
        mat *coefficients = dataset_->principal_components_data()->coeff();
        table_->setModel(new VespucciTableModel(this, coefficients));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "Eigenvalues of Covariance Matrix"){
        mat *latent = (mat*) dataset_->principal_components_data()->latent();
        table_->setModel(new VespucciTableModel(this, latent));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PCA t² Values"){
        mat *tsquared = (mat*) dataset_->principal_components_data()->tsquared();
        table_->setModel(new VespucciTableModel(this, tsquared));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "VCA Coefficients"){
        mat* coeff = dataset_->vertex_components_data()->fractional_abundances();
        table_->setModel(new VespucciTableModel(this, coeff));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "VCA Endmembers"){
        mat *endmembers = dataset_->vertex_components_data()->endmember_spectra();
        table_->setModel(new VespucciTableModel(this, endmembers));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(false);
    }
    else if (arg1 == "K-means Assignments"){
        mat *assignments = dataset_->k_means_data();
        table_->setModel(new VespucciTableModel(this, assignments));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Variance"){
        mat *variance = dataset_->partial_least_squares_data()->percent_variance();
        table_->setModel(new VespucciTableModel(this, variance));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Predictor Loading"){
        mat *XL = dataset_->partial_least_squares_data()->X_loadings();
        table_->setModel(new VespucciTableModel(this, XL));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Response Loading"){
        mat *YL = dataset_->partial_least_squares_data()->Y_loadings();
        table_->setModel(new VespucciTableModel(this, YL));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Predictor Scores"){
        mat *XS = dataset_->partial_least_squares_data()->X_scores();
        table_->setModel(new VespucciTableModel(this, XS));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Response Scores"){
        mat *YS = dataset_->partial_least_squares_data()->Y_scores();
        table_->setModel(new VespucciTableModel(this, YS));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else if (arg1 == "PLS Coefficients"){
        mat *coeff = dataset_->partial_least_squares_data()->coefficients();
        table_->setModel(new VespucciTableModel(this, coeff));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }
    else{
        cout << "Else" << endl;
        table_->setModel(new VespucciTableModel(this, dataset_->wavelength_ptr()));
        export_button_->setDisabled(false);
        plot_button_->setDisabled(true);
    }

    table_->resizeColumnsToContents();
    table_->resizeRowsToContents();
}


void DataViewer::on_plotPushButton_clicked()
{
    if (current_text_ == "VCA Endmembers"){
        bool ok;
        int number = current_data_->n_cols;
        int endmember = QInputDialog::getInt(this, tr("Select Endmember"), tr("Endmember"), 1, 1, number, 1, ok, 0);
        endmember--; //fix indexing from human-intuitve 1 to programmer-intuitive 0
        if (ok){
            spectrum_viewer_ = new SpectrumViewer(this, dataset_, endmember, directory_, tr("VCA"));
            spectrum_viewer_->show();
        }
    }
}
