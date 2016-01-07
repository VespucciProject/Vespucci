#include "classicalleastsquaresdialog.h"
#include "ui_classicalleastsquaresdialog.h"

ClassicalLeastSquaresDialog::ClassicalLeastSquaresDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::ClassicalLeastSquaresDialog)
{
    ui->setupUi(this);
    dataset_combo_box_ = findChild<QComboBox *>("datasetComboBox");
    dataset_radio_button_ = findChild<QRadioButton *>("datasetRadioButton");
    file_radio_button_ = findChild<QRadioButton *>("fileRadioButton");
    filename_line_edit_ = findChild<QLineEdit *>("filenameLineEdit");
    component_spin_box_ = findChild<QSpinBox *>("componentSpinBox");
    gradient_combo_box_ = findChild<QComboBox *>("gradientComboBox");
    image_check_box_ = findChild<QCheckBox *>("imageCheckBox");
    name_line_edit_= findChild<QLineEdit *>("nameLineEdit");
    browse_push_button_ = findChild<QPushButton *>("browsePushButton");

    gradient_combo_box_->setDisabled(true);
    file_radio_button_->setChecked(true);
    dataset_radio_button_->setChecked(false);
    dataset_combo_box_->setDisabled(true);
    component_spin_box_->setDisabled(true);
    gradient_combo_box_->setDisabled(true);
    name_line_edit_->setDisabled(true);

    workspace = ws;
    dataset_ = workspace->DatasetAt(row);
    QStringList dataset_names = workspace->dataset_names();
    dataset_combo_box_->addItems(dataset_names);

}

ClassicalLeastSquaresDialog::~ClassicalLeastSquaresDialog()
{
    delete ui;
}

void ClassicalLeastSquaresDialog::on_datasetRadioButton_clicked()
{
    dataset_combo_box_->setEnabled(true);
    file_radio_button_->setChecked(false);
    filename_line_edit_->setDisabled(true);
    browse_push_button_->setDisabled(true);
}

void ClassicalLeastSquaresDialog::on_fileRadioButton_clicked()
{
    filename_line_edit_->setEnabled(true);
    browse_push_button_->setEnabled(true);
    dataset_combo_box_->setDisabled(true);
    dataset_radio_button_->setChecked(false);
}

void ClassicalLeastSquaresDialog::on_imageCheckBox_stateChanged(int arg1)
{
    component_spin_box_->setEnabled(arg1);
    gradient_combo_box_->setEnabled(arg1);
    name_line_edit_->setEnabled(arg1);

}

void ClassicalLeastSquaresDialog::on_buttonBox_accepted()
{
    QString filename;
    int dataset_row;
    mat control_spectra;
    QSharedPointer<VespucciDataset> control_dataset;
    if (dataset_radio_button_->isChecked()){
        dataset_row = dataset_combo_box_->currentIndex();
        control_dataset = workspace->DatasetAt(dataset_row);
        control_spectra = control_dataset->spectra();
    }
    else if (file_radio_button_->isChecked()){
        filename = filename_line_edit_->text();
        control_spectra.load(filename.toStdString());
    }
    else{return;}
    if (image_check_box_->isChecked()){
        int gradient_index = gradient_combo_box_->currentIndex();
        int component = component_spin_box_->value();
        if (component > control_spectra.n_cols)
            component = control_spectra.n_cols;
        component--; //accounting for indexing at 0.
        QString name = name_line_edit_->text();
        try{
            dataset_->ClassicalLeastSquares(control_spectra, component, name, gradient_index);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else{
        QString name = "Classical Least Squares";
        try{
            dataset_->ClassicalLeastSquares(control_spectra, name);
        }catch(exception e){
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }

}

void ClassicalLeastSquaresDialog::on_browsePushButton_clicked()
{
    QString filename =
            QFileDialog::getOpenFileName(this,
                                         "Select Control File",
                                         workspace->directory(),
                                         "Data files (.csv .txt .arma);; "
                                         "All Files");
    filename_line_edit_->setText(filename);
}
