#include "bandratiodialog.h"
#include "ui_bandratiodialog.h"

BandRatioDialog::BandRatioDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::BandRatioDialog)
{
    ui->setupUi(this);


    workspace = ws;
    spectrum_plot_ = this->findChild<QCustomPlot *>("spectrumPlot");
    first_min_box_ = this->findChild<QLineEdit *>("firstMinLineEdit");
    first_max_box_ = this->findChild<QLineEdit *>("firstMaxLineEdit");
    second_min_box_ = this->findChild<QLineEdit *>("secondMinLineEdit");
    second_max_box_ = this->findChild<QLineEdit *>("secondMaxLineEdit");

    name_box_ = this->findChild<QLineEdit *>("nameLineEdit");
    color_selector_ = this->findChild<QComboBox *>("gradientComboBox");
    value_method_selector_ = this->findChild<QComboBox *>("peakComboBox");


    data_index_ = row;

    data_ = workspace->DatasetAt(row);
}

BandRatioDialog::~BandRatioDialog()
{
    delete ui;
}
