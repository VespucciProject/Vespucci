#include "classicaleastsquaresdialog.h"
#include "ui_classicaleastsquaresdialog.h"

ClassicaLeastSquaresDialog::ClassicaLeastSquaresDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::ClassicaLeastSquaresDialog)
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

    dataset_combo_box_->addItems(workspace->dataset_names());

}

ClassicaLeastSquaresDialog::~ClassicaLeastSquaresDialog()
{
    delete ui;
}

void ClassicaLeastSquaresDialog::on_datasetRadioButton_clicked()
{
    dataset_combo_box_->setEnabled(true);
    file_radio_button_->setChecked(false);
    filename_line_edit_->setDisabled(true);
    browse_push_button_->setDisabled(true);
}

void ClassicaLeastSquaresDialog::on_fileRadioButton_clicked()
{
    filename_line_edit_->setEnabled(true);
    browse_push_button_->setEnabled(true);
    dataset_combo_box_->setDisabled(true);
    dataset_radio_button_->setChecked(false);
}

void ClassicaLeastSquaresDialog::on_imageCheckBox_stateChanged(int arg1)
{
    component_spin_box_->setEnabled(arg1);
    gradient_combo_box_->setEnabled(arg1);
    name_line_edit_->setEnabled(arg1);

}

void ClassicaLeastSquaresDialog::on_buttonBox_accepted()
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
