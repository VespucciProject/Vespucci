#include "GUI/Display/statsdialog.h"
#include "ui_statsdialog.h"
#include <Math/Stats/confidenceinterval.h>
#include "Global/global.h"

StatsDialog::StatsDialog(MainWindow *parent, QSharedPointer<VespucciWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::StatsDialog)
{
    workspace_ = ws;
    ui->setupUi(this);
    hist_plot_ = new QCPBars(ui->histogramCustomPlot->xAxis,
                              ui->histogramCustomPlot->yAxis);

    ui->histogramCustomPlot->addPlottable(hist_plot_);

}

StatsDialog::~StatsDialog()
{
    delete ui;
}

void StatsDialog::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev);
    UpdateDisplayData();
}

void StatsDialog::closeEvent(QCloseEvent *ev)
{
    QDialog::closeEvent(ev);
    emit SetActionChecked(false);
}

void StatsDialog::MatrixSelectionChanged(QStringList matrix_keys)
{
    data_keys_ = matrix_keys;
    if (isVisible()) UpdateDisplayData();
}

void StatsDialog::MatrixToBeRemoved(QStringList matrix_keys)
{
    if (Vespucci::KeysAreEqual(data_keys_, matrix_keys)){
        data_keys_ = QStringList();
        ClearFields();
    }

}

void StatsDialog::DatasetToBeRemoved(QString name)
{
    if (data_keys_.first() == name){
        data_keys_ = QStringList();
        ClearFields();
    }
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
    hist_plot_->setData(hist_abs_qvec, hist_data_qvec);
    ui->histogramCustomPlot->rescaleAxes();
    ui->histogramCustomPlot->replot();
}

void StatsDialog::UpdateDisplayData()
{
    if (data_keys_.size() < 2) ClearFields();
    GenerateHistogram();
    CalculateCI();
    const mat& data = workspace_->GetMatrix(data_keys_);
    ui->minLineEdit->setText(QString::number(data.min()));
    ui->maxLineEdit->setText(QString::number(data.max()));
    ui->medLineEdit->setText(QString::number(CalculateMedian()));
    ui->stddevLineEdit->setText(QString::number(CalculateStdDev()));
    ui->meanLineEdit->setText(QString::number(CalculateMean()));
    ui->nameLabel->setText(data_keys_.last());
    QString dimensions = QString::number(data.n_rows) + "×" + QString::number(data.n_cols);
    ui->dimensionLabel->setText(dimensions);
    ui->plottableLabel->setText(workspace_->Plottable(data_keys_) ? "True" : "False");
    ui->mappableLabel->setText(workspace_->Mappable(data_keys_) ? "True" : "False");
}

void StatsDialog::CalculateCI()
{
    double alpha = ui->alphaDoubleSpinBox->value();
    double stddev = CalculateStdDev();
    const mat& data = workspace_->GetMatrix(data_keys_);
    unsigned int n = data.n_elem;
    double w = Vespucci::Math::Stats::TInterval(alpha, stddev, n);
    ui->confidenceLineEdit->setText(QString::number(w));
}


void StatsDialog::on_calculatePushButton_clicked()
{
    CalculateCI();
}

void StatsDialog::ClearFields()
{
    ui->minLineEdit->setText("NA");
    ui->maxLineEdit->setText("NA");
    ui->medLineEdit->setText("NA");
    ui->stddevLineEdit->setText("NA");
    ui->meanLineEdit->setText("NA");
    ui->nameLabel->setText("No matrix selected");
    ui->dimensionLabel->setText("NA");
    ui->plottableLabel->setText("NA");
    ui->mappableLabel->setText("NA");
    ui->confidenceLineEdit->setText("NA");
    ui->histogramCustomPlot->clearPlottables();
    ui->histogramCustomPlot->replot();
}
