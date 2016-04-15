#include "GUI/Display/statsdialog.h"
#include "ui_statsdialog.h"

StatsDialog::StatsDialog(MainWindow *parent, VespucciWorkspace *ws) :
    QDialog(parent),
    ui(new Ui::StatsDialog)
{
    workspace = ws;
    ui->setupUi(this);
    min_line_edit_ = findChild<QLineEdit *>("minLineEdit");
    max_line_edit_ = findChild<QLineEdit *>("maxLineEdit");
    median_line_edit_ = findChild<QLineEdit *>("medLineEdit");
    stddev_line_edit_ = findChild<QLineEdit *>("stddevLineEdit");
    mean_line_edit_ = findChild<QLineEdit *>("meanLineEdit");
    histogram_custom_plot_ = findChild<QCustomPlot *>("histogramCustomPlot");
}

void StatsDialog::SetActiveDataKeys(const QStringList &keys)
{
    data_keys_ = keys;
    UpdateDisplayData();
}

StatsDialog::~StatsDialog()
{
    delete ui;
}

void StatsDialog::on_buttonBox_accepted()
{
    close();
}

double StatsDialog::CalculateMedian()
{
    const mat& data = workspace->GetMatrix(data_keys_);
    if (!data.n_elem) return 0;
    if (data.n_cols == 1) return median(data);
    if (data.n_cols > 1){
        mat data_copy = data;
        data_copy.reshape(data.n_elem, 1);
        return median(data_copy);
    }

}

double StatsDialog::CalculateStdDev()
{
    const mat& data = workspace->GetMatrix(data_keys_);
    if (!data.n_elem) return 0;
    if (data.n_cols == 1) return stddev(data);
    if (data.n_cols > 1){
        mat data_copy = data;
        data_copy.reshape(data.n_elem, 1);
        return stddev(data_copy);
    }
}

double StatsDialog::CalculateMean()
{
    const mat& data = workspace->GetMatrix(data_keys_);
    if (!data.n_elem) return 0;
    if (data.n_cols == 1) return mean(data);
    if (data.n_cols > 1){
        mat data_copy = data;
        data_copy.reshape(data.n_elem, 1);
        return mean(data_copy);
    }
}

void StatsDialog::GenerateHistogram()
{
    if (!data.n_elem) return 0;
    const mat& data = workspace->GetMatrix(data_keys_);
    //Sturges' rule for finding bin counts automagically
    uword bin_count = std::round(1.0 + 3.332*std::log10((double) data.n_elem));
    double range = data.max() - data.min();
    double bin_width = range / ((double) bin_count);
    vec edges(bin_count);
    bin_count(0) = data.min();
    for (uword i = 0; i < bin_count; ++i)
        bin_count(i) = data.min() + bin_width*((double) i);
    vec hist_data;
    if (data.n_cols == 1){hist_data = histc(data, edges);}
    else{
        mat data_copy = data;
        data_copy.reshape(data.n_elem, 1);
        hist_data = histc(data, edges);
    }
    hist_abs = edges.rows(1,edges.n_rows - 2);

    QVector<double> hist_data_qvec =
            QVector<double>::fromStdVector(conv_to<vector<double> >::from(hist_data));
    QVector<double> hist_abs_qvec =
            QVector<double>::fromStdVector(conv_to<vector<double> >::from(edges.rows(1, edges.n_rows - 2)));

    QCPBars *hist_plot = new QCPBars(histogram_custom_plot_->xAxis,
                                     histogram_custom_plot_->yAxis);
    hist_plot->setData(hist_abs_qvec, hist_data_qvec);
    histogram_custom_plot_->addPlottable(hist_plot);
}

void StatsDialog::UpdateDisplayData()
{
    GenerateHistogram();
    const mat& data = workspace->GetMatrix(data_keys_);
    min_line_edit_->setText(QString::number(data_.min()));
    max_line_edit_->setText(QString::number(data_.max()));
    median_line_edit_->setText(QString::number(CalcualteMedian()));
    stddev_line_edit_->setText(QString::number(CalcualteStdDev()));
    mean_line_edit_->setText(QString::number(CalculateMean()));
}

void StatsDialog::CalcualteCI()
{

}

