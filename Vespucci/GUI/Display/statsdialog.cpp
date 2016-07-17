#include "GUI/Display/statsdialog.h"
#include "ui_statsdialog.h"
#include <Math/Stats/confidenceinterval.h>
#include <Math/Stats/histogram.h>
#include "Global/global.h"

StatsDialog::StatsDialog(MainWindow *parent, QSharedPointer<VespucciWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::StatsDialog)
{
    workspace_ = ws;
    ui->setupUi(this);
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
    ui->histogramCustomPlot->clearPlottables();
    const mat& data = workspace_->GetMatrix(data_keys_);
    if (!data.n_elem) return;
    vec edges;
    uvec hist = Vespucci::Math::Stats::GenerateHistogram(data, edges);
    edges.shed_row(edges.n_rows - 1);

    qvec histq = qvec::fromStdVector(conv_to<stdvec>::from(hist));
    qvec edgesq = qvec::fromStdVector(conv_to<stdvec>::from(edges));

    QCPBars *hist_plot = new QCPBars(ui->histogramCustomPlot->xAxis,
                                     ui->histogramCustomPlot->yAxis);
    hist_plot->addData(edgesq, histq);
    ui->histogramCustomPlot->addPlottable(hist_plot);
    ui->histogramCustomPlot->rescaleAxes();
    ui->histogramCustomPlot->replot();
}

void StatsDialog::UpdateDisplayData()
{
    if (data_keys_.size() < 2) ClearFields();
    GenerateHistogram();
    CalculateCI();
    const mat& data = workspace_->GetMatrix(data_keys_);
    if (!data.n_elem){
        ClearFields();
        return;
    }
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
    const mat& data = workspace_->GetMatrix(data_keys_);
    if (!data.n_elem) return;
    double alpha = ui->alphaDoubleSpinBox->value();
    double stddev = CalculateStdDev();
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
