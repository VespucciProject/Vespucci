#include "abscissainterpolationdialog.h"
#include "ui_abscissainterpolationdialog.h"
#include "GUI/waitdialog.h"
AbscissaInterpolationDialog::AbscissaInterpolationDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::AbscissaInterpolationDialog)
{
    ui->setupUi(this);
    workspace = ws;
    dataset_ = workspace->DatasetAt(row);

    dataset_combo_box_ = findChild<QComboBox*>("datasetComboBox");
    dataset_radio_button_ = findChild<QRadioButton*>("datasetRadioButton");
    filename_line_edit_ = findChild<QLineEdit*>("filenameLineEdit");
    browse_push_button_ = findChild<QPushButton*>("browsePushButton");
    method_combo_box_ = findChild<QComboBox *>("methodComboBox");
    order_spin_box_ = findChild<QSpinBox *>("orderSpinBox");
    window_spin_box_ = findChild<QSpinBox *>("windowSpinBox");
    source_combo_box_ = findChild<QComboBox*>("sourceComboBox");

    window_label_ = findChild<QLabel *>("windowLabel");
    order_label_ = findChild<QLabel *>("orderLabel");
    file_label_ = findChild<QLabel *>("fileLabel");
    dataset_label_=  findChild<QLabel *>("datasetLabel");

    order_spin_box_->setVisible(false);
    order_label_->setVisible(false);
    window_spin_box_->setVisible(false);
    window_label_->setVisible(false);

    dataset_combo_box_->setVisible(true);
    dataset_label_->setVisible(true);
    file_label_->setVisible(false);
    browse_push_button_->setVisible(false);
    filename_line_edit_->setVisible(false);

    QStringList dataset_names = workspace->dataset_names();
    dataset_combo_box_->addItems(dataset_names);

}

AbscissaInterpolationDialog::~AbscissaInterpolationDialog()
{
    delete ui;
}


void AbscissaInterpolationDialog::on_buttonBox_accepted()
{
    arma::vec new_abscissa;
    if (source_combo_box_->currentText() == "Dataset"){
        new_abscissa =
                workspace->DatasetAt(dataset_combo_box_->currentIndex())->abscissa();
    }
    else if (source_combo_box_->currentText() == "File"){
        arma::mat infile;
        bool ok = infile.load(filename_line_edit_->text().toStdString());
        if (ok){
            new_abscissa = infile.col(0);
        }
        else{
            QMessageBox::warning(this, "Error Opening File",
                                 "The file " + filename_line_edit_->text()\
                                 + " could not be opened");\
            return;
        }
    }
    else{dataset_.clear(); return;}
    QProgressDialog progress(this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setLabelText("Interpolating Spectra...");
    progress.setCancelButton(0);
    progress.setRange(0,0);
    progress.exec();
    if (method_combo_box_->currentText() == "Two-Point Linear"){
        try{
            dataset_->InterpolateToNewAbscissa(new_abscissa);
        }catch(exception e){
            progress.close();
            workspace->main_window()->DisplayExceptionWarning(e);
        }
    }
    else if (method_combo_box_->currentText() == "Spline"){
        int poly_order = order_spin_box_->value();
        int window_size = window_spin_box_->value();
        try{
            dataset_->InterpolateToNewAbscissa(new_abscissa,
                                               poly_order,
                                               window_size);
        }catch(exception e){
            progress.close();
            workspace->main_window()->DisplayExceptionWarning(e);
        }

    }
    else{
        progress.close();
        dataset_.clear();
        return;
    }
    progress.close();
    dataset_.clear();

}

void AbscissaInterpolationDialog::on_sourceComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Dataset"){
        dataset_combo_box_->setVisible(true);
        dataset_label_->setVisible(true);
        file_label_->setVisible(false);
        browse_push_button_->setVisible(false);
        filename_line_edit_->setVisible(false);
    }

    if (arg1 == "File"){
        dataset_combo_box_->setVisible(false);
        dataset_label_->setVisible(false);
        file_label_->setVisible(true);
        browse_push_button_->setVisible(true);
        filename_line_edit_->setVisible(true);
    }
}

void AbscissaInterpolationDialog::on_methodComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Two-Point Linear"){
        order_spin_box_->setVisible(false);
        order_label_->setVisible(false);
        window_spin_box_->setVisible(false);
        window_label_->setVisible(false);
    }
    if (arg1 == "Spline"){
        order_spin_box_->setVisible(true);
        order_label_->setVisible(true);
        window_spin_box_->setVisible(true);
        window_label_->setVisible(true);
    }
}
