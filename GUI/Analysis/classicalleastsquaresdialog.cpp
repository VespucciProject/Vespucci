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
    name_line_edit_= findChild<QLineEdit *>("nameLineEdit");
    browse_push_button_ = findChild<QPushButton *>("browsePushButton");

    file_radio_button_->setChecked(true);
    dataset_radio_button_->setChecked(false);
    dataset_combo_box_->setDisabled(true);
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
