#include "GUI/Analysis/plotmakerdialog.h"
#include "ui_plotmakerdialog.h"
#include "GUI/Display/plotviewer.h"

PlotMakerDialog::PlotMakerDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::PlotMakerDialog)
{
    ui->setupUi(this);
    x_spin_box_ = findChild<QSpinBox *>("xSpinBox");
    y_spin_box_ = findChild<QSpinBox *>("ySpinBox");
    mapping_combo_box_ = findChild<QComboBox *>("mappingComboBox");
    variables_combo_box_ = findChild<QComboBox *>("variablesComboBox");
    workspace = ws;
    dataset_ = workspace->DatasetAt(row);

    QStringList variable_items;
    QStringList mapping_items;
    if (dataset_->principal_components_calculated()){
        variable_items.append("Principal Components Scores");
        variable_items.append("Principal Components Loadings");
    }
    if (dataset_->partial_least_squares_calculated()){
        variable_items.append("PLS Scores");
        variable_items.append("PLS Loadings");
    }
    if (dataset_->vertex_components_calculated()){
        variable_items.append("VCA Scores");
    }
    if (dataset_->meta()){
        mapping_items.append("Parent Dataset Indices");
    }
    mapping_combo_box_->addItems(mapping_items);
    variables_combo_box_->addItems(variable_items);
}

PlotMakerDialog::~PlotMakerDialog()
{
    delete ui;
}

void PlotMakerDialog::on_buttonBox_accepted()
{
    arma::vec x, y;
    arma::uword x_ind = x_spin_box_->value();
    arma::uword y_ind = y_spin_box_->value();
    QString x_label, y_label;
    QString variable_description = variables_combo_box_->currentText();
    QString map_var_description = mapping_combo_box_->currentText();
    bool valid;
    if (variable_description == "PLS Scores"){
        x = dataset_->partial_least_squares_data()->Results(x_ind, valid);
        y = dataset_->partial_least_squares_data()->Results(y_ind, valid);
        x_label = "PLS Score " + QString::number(x_ind);
        y_label = "PLS Score " + QString::number(y_ind);
    }
    else if (variable_description == "PLS Loadings"){
        x = dataset_->partial_least_squares_data()->X_loadings()->col(x_ind);
        y = dataset_->partial_least_squares_data()->X_loadings()->col(y_ind);
        x_label = "PLS Loading " + QString::number(x_ind);
        y_label = "PLS Loading " + QString::number(y_ind);
    }
    else if (variable_description == "PCA Scores"){
        x = dataset_->principal_components_data()->score()->row(x_ind);
        y = dataset_->principal_components_data()->score()->row(y_ind);
        x_label = "PCA Score " + QString::number(x_ind);
        y_label = "PCA Score " + QString::number(y_ind);
    }
    else if (variable_description == "PCA Loadings"){
        x = dataset_->principal_components_data()->coeff()->row(x_ind);
        y = dataset_->principal_components_data()->coeff()->row(y_ind);
        x_label = "PCA Loading " + QString::number(x_ind);
        y_label = "PCA Loading " + QString::number(y_ind);
    }
    else if (variable_description == "VCA Scores"){
        x = dataset_->vertex_components_data()->Results(x_ind);
        y = dataset_->vertex_components_data()->Results(y_ind);
        x_label = "VCA Score " + QString::number(x_ind);
        y_label = "VCA Score " + QString::number(y_ind);
    }
    else{
        x_spin_box_->setMaximum(dataset_->abscissa_ptr()->n_rows);
        y_spin_box_->setMaximum(dataset_->abscissa_ptr()->n_rows);
        close();
    }

    try{
        if (map_var_description == "Import Categorical Data"){
            mat z;
            QString filename = QFileDialog::getOpenFileName(this, "Select File",
                                                            workspace->directory(),
                                                            "Text Files *.txt *.csv");
            z.load(filename.toStdString());
            if (z.n_rows == x.n_rows){
                PlotViewer *plot_view = new PlotViewer(this, x, y, z.col(0), x_label, y_label);
                plot_view->SetDirectory(workspace->directory());
                plot_view->SetLineStyle(QCPGraph::lsNone);
                plot_view->show();

            }
        }
        else if (map_var_description == "Parent Dataset Indices"){
            vec z = dataset_->parent_dataset_indices()->col(0);
            PlotViewer *plot_view = new PlotViewer(this, x, y, z.col(0), x_label, y_label);
            plot_view->SetDirectory(workspace->directory());
            plot_view->SetLineStyle(QCPGraph::lsNone);
            plot_view->show();
        }
        else{
            PlotViewer *plot_view = new PlotViewer(this, x, y, x_label, y_label);
            plot_view->SetDirectory(workspace->directory());
            plot_view->SetLineStyle(QCPGraph::lsNone);
            plot_view->show();
        }
    }catch(exception e){
        workspace->main_window()->DisplayExceptionWarning(e);
    }

    close();
}

void PlotMakerDialog::on_variablesComboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "PLS Scores"){
        x_spin_box_->setMaximum(dataset_->partial_least_squares_data()->NumberComponents());
        y_spin_box_->setMaximum(dataset_->partial_least_squares_data()->NumberComponents());
    }
    else if (arg1 == "VCA Scores"){
        x_spin_box_->setMaximum(dataset_->vertex_components_data()->NumberComponents());
        y_spin_box_->setMaximum(dataset_->vertex_components_data()->NumberComponents());
    }
    else{
        x_spin_box_->setMaximum(dataset_->abscissa_ptr()->n_rows);
        y_spin_box_->setMaximum(dataset_->abscissa_ptr()->n_rows);
    }
}
