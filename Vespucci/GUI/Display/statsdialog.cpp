#include "GUI/Display/statsdialog.h"
#include "ui_statsdialog.h"
#include <Math/Stats/confidenceinterval.h>

StatsDialog::StatsDialog(MainWindow *parent, QSharedPointer<VespucciWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::StatsDialog)
{
    workspace_ = ws;
    ui->setupUi(this);
    min_line_edit_ = findChild<QLineEdit *>("minLineEdit");
    max_line_edit_ = findChild<QLineEdit *>("maxLineEdit");
    median_line_edit_ = findChild<QLineEdit *>("medLineEdit");
    stddev_line_edit_ = findChild<QLineEdit *>("stddevLineEdit");
    mean_line_edit_ = findChild<QLineEdit *>("meanLineEdit");
    histogram_custom_plot_ = findChild<QCustomPlot *>("histogramCustomPlot");
    dimension_label_ = findChild<QLabel *>("dimensionLabel");
    plottable_label_ = findChild<QLabel *>("plottableLabel");
    mappable_label_ = findChild<QLabel *>("mappableLabel");
    name_label_ = findChild<QLabel *>("nameLabel");
    alpha_double_spin_box_ = findChild<QDoubleSpinBox *>("alphaDoubleSpinBox");
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
    const mat& data = workspace_->GetMatrix(data_keys_);
    if (!data.n_elem) return 0;
    if (data.n_cols == 1) return as_scalar(median(data));
    if (data.n_cols > 1){
        mat data_copy = data;
        data_copy.reshape(data.n_elem, 1);
        return as_scalar(median(data_copy));
    }
    return 0;

}

double StatsDialog::CalculateStdDev()
{
    const mat& data = workspace_->GetMatrix(data_keys_);
    if (!data.n_elem) return 0;
    if (data.n_cols == 1) return as_scalar(stddev(data));
    if (data.n_cols > 1){
        mat data_copy = data;
        data_copy.reshape(data.n_elem, 1);
        return as_scalar(stddev(data_copy));
    }
    return 0;
}

double StatsDialog::CalculateMean()
{
    const mat& data = workspace_->GetMatrix(data_keys_);
    if (!data.n_elem) return 0;
    if (data.n_cols == 1) return as_scalar(mean(data));
    if (data.n_cols > 1){
        mat data_copy = data;
        data_copy.reshape(data.n_elem, 1);
        return as_scalar(mean(data_copy));
    }
    return 0;
}

void StatsDialog::GenerateHistogram()
{
    const mat& data = workspace_->GetMatrix(data_keys_);
    if (!data.n_elem) return;
    //Sturges' rule for finding bin counts automagically
    uword bin_count = std::round(1.0 + 3.332*std::log10((double) data.n_elem));
    double range = data.max() - data.min();
    double bin_width = range / ((double) bin_count);
    vec edges(bin_count);
    edges(0) = data.min();
    for (uword i = 0; i < bin_count; ++i)
        edges(i) = data.min() + bin_width*(double(i));
    uvec hist_data;
    if (data.n_cols == 1){hist_data = histc(data, edges);}
    else{
        mat data_copy = data;
        data_copy.reshape(data.n_elem, 1);
        hist_data = histc(data, edges);
    }

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
    CalculateCI();
    const mat& data = workspace_->GetMatrix(data_keys_);
    min_line_edit_->setText(QString::number(data.min()));
    max_line_edit_->setText(QString::number(data.max()));
    median_line_edit_->setText(QString::number(CalculateMedian()));
    stddev_line_edit_->setText(QString::number(CalculateStdDev()));
    mean_line_edit_->setText(QString::number(CalculateMean()));
    name_label_->setText(data_keys_.last());
    QString dimensions = QString::number(data.n_rows) + "×" + QString::number(data.n_cols);
    dimension_label_->setText(dimensions);
    plottable_label_->setText(workspace_->Plottable(data_keys_) ? "True" : "False");
    mappable_label_->setText(workspace_->Mappable(data_keys_) ? "True" : "False");
}

void StatsDialog::CalculateCI()
{
    double alpha = alpha_double_spin_box_->value();
    double stddev = CalculateStdDev();
    const mat& data = workspace_->GetMatrix(data_keys_);
    unsigned int n = data.n_elem;
    double w = Vespucci::Math::Stats::TInterval(alpha, stddev, n);
    confidence_line_edit_->setText(QString::number(w));
}


void StatsDialog::on_calculatePushButton_clicked()
{
    CalculateCI();
}
