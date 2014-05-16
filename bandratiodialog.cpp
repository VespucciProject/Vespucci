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

    double min = workspace->GetWavelengthMin(row);
    double max = workspace->GetWavelengthMax(row);

    QDoubleValidator *validator = new QDoubleValidator(min, max, 2, this);

    first_min_box_->setValidator(validator);
    first_max_box_->setValidator(validator);
    second_min_box_->setValidator(validator);
    second_max_box_->setValidator(validator);

    data_ = workspace->DatasetAt(row);

    QVector<double> plot_data = data_->PointSpectrum(0);
    QVector<double> wavelength = data_->WavelengthQVector();

    spectrum_plot_->addGraph();
    spectrum_plot_->graph(0)->addData(wavelength, plot_data);


    spectrum_plot_->xAxis->setRange(data_->WavelengthRange());
    spectrum_plot_->yAxis->setRange(data_->PointSpectrumRange(0));
}

BandRatioDialog::~BandRatioDialog()
{
    delete ui;
}

void BandRatioDialog::on_buttonBox_accepted()
{
    double first_entered_min = first_min_box_->text().toDouble();
    double second_entered_min = second_min_box_->text().toDouble();
    double first_entered_max = first_max_box_->text().toDouble();
    double second_entered_max = second_max_box_->text().toDouble();

    vector<int> first_range = data_->FindRange(first_entered_min, first_entered_max);
    vector<int> second_range = data_->FindRange(second_entered_min, second_entered_max);

    int first_min = first_range[0];
    int first_max = first_range[1];
    int second_min = second_range[0];
    int second_max = second_range[1];

    QString name = name_box_->text();
    QString value_method = value_method_selector_->currentText();
    int color_index = color_selector_->currentIndex();

    data_->BandRatio(first_min,
                     first_max,
                     second_min,
                     second_max,
                     name,
                     value_method,
                     color_index);
}
