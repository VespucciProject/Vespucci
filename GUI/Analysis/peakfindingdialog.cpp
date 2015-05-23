#include "peakfindingdialog.h"
#include "ui_peakfindingdialog.h"
#include "Data/Analysis/cwtdata.h"
PeakFindingDialog::PeakFindingDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::PeakFindingDialog)
{
    ui->setupUi(this);
    dataset_ = ws->DatasetAt(row);

    save_coefs_box_ = findChild<QCheckBox *>("coefCheckBox");
    gradient_box_ = findChild<QComboBox *>("gradientComboBox");
    method_box_ =findChild<QComboBox *>("methodComboBox");
    save_coef_plots_box_ = findChild<QCheckBox *>("coefPlotCheckBox");
    directory_line_edit_ = findChild<QLineEdit *>("directoryLineEdit");
    method_box_ = findChild<QComboBox *>("methodComboBox");
    save_ridges_box_ = findChild<QCheckBox *>("ridgeCheckBox");
    save_ridge_plots_box_ = findChild<QCheckBox *>("ridgePlotCheckBox");
    width_box_ = findChild<QCheckBox *>("widthCheckBox");


}

PeakFindingDialog::~PeakFindingDialog()
{
    delete ui;
}

void PeakFindingDialog::on_browseButton_clicked()
{
    QString directory = workspace->directory();
    QString save_directory = QFileDialog::getExistingDirectory(this, "Select Save Directory", directory);
    directory_line_edit_->setText(save_directory);
}

void PeakFindingDialog::on_buttonBox_accepted()
{
    CWTData *cwt_data = new CWTData(dataset_);
    QString save_path = directory_line_edit_->text();
    int method_num = method_box_->currentIndex();
    bool estimate_width = width_box_->isChecked();
    bool save_coefs = save_coef_plots_box_->isChecked();
    bool save_coef_plots = save_coef_plots_box_->isChecked();
    bool save_ridges = save_ridges_box_->isChecked();
    bool save_ridge_plots = save_ridge_plots_box_->isChecked();
    QCPColorGradient gradient = dataset_->GetGradient(gradient_box_->currentIndex());

    cwt_data->Apply("mexh",
                    70, 3, 5, 5, 3.0, "quantile", 500,
                    save_coefs, save_coef_plots,
                    save_ridge_plots, save_ridges,
                    estimate_width,
                    save_path, "pdf",
                    gradient);
}
