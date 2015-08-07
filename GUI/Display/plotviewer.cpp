#include "plotviewer.h"
#include "ui_plotviewer.h"

PlotViewer::PlotViewer(QWidget *parent, const arma::vec &x, const arma::vec &y, arma::vec z, QString x_label, QString y_label) :
    QDialog(parent),
    ui(new Ui::PlotViewer)
{
    ui->setupUi(this);
    typedef std::vector<double> stdvec;
    arma::field<arma::vec> x_parts;
    arma::field<arma::vec> y_parts;
    arma::vec unique_z = arma::unique(z);
    x_parts.set_size(unique_z.n_rows);
    y_parts.set_size(unique_z.n_rows);
    QVector<QColor> colors(unique_z.n_rows);
    colors.append(QColor(228,26,28));
    colors.append(QColor(55,126,184));
    colors.append(QColor(77,175,74));
    colors.append(QColor(152,78,163));
    colors.append(QColor(255,127,0));
    colors.append(QColor(255,255,51));
    colors.append(QColor(166,86,40));
    colors.append(QColor(247,129,191));
    colors.append(QColor(153,153,153));

    plot_->xAxis->setLabel(x_label);
    plot_->yAxis->setLabel(y_label);
    plot_->xAxis->setRange(x.min(), x.max());
    plot_->yAxis->setRange(y.min(), y.max());
    for (arma::uword i = 0; i < unique_z.n_rows; ++i){
        QPen pen(colors[i]);
        arma::uvec ind = arma::find(z == unique_z(i));
        QVector<double> x_qvec = QVector<double>::fromStdVector(arma::conv_to<stdvec>::from(x(ind)));
        QVector<double> y_qvec = QVector<double>::fromStdVector(arma::conv_to<stdvec>::from(y(ind)));
        plot_->addGraph();
        plot_->graph(i)->setName(QString::number(i));
        plot_->graph(i)->setData(x_qvec, y_qvec);
        plot_->graph(i)->setPen(pen);
        plot_->legend->setVisible(true);
        plot_->graph(i)->addToLegend();
    }
    plot_->replot();
}

PlotViewer::PlotViewer(QWidget *parent, const arma::vec &x, const arma::vec &y, QString x_label, QString y_label) :
    QDialog(parent),
    ui(new Ui::PlotViewer)
{
    ui->setupUi(this);
    plot_ = findChild<QCustomPlot *>("plot");

    typedef std::vector<double> stdvec;
    QVector<double> x_qvec = QVector<double>::fromStdVector(arma::conv_to<stdvec>::from(x));
    QVector<double> y_qvec = QVector<double>::fromStdVector(arma::conv_to<stdvec>::from(y));
    plot_->addGraph();
    plot_->graph(0)->setData(x_qvec, y_qvec);
    plot_->xAxis->setLabel(x_label);
    plot_->yAxis->setLabel(y_label);
    plot_->xAxis->setRange(x.min(), x.max());
    plot_->yAxis->setRange(y.min(), y.max());
    plot_->replot();
}

PlotViewer::~PlotViewer()
{
    delete ui;
}
