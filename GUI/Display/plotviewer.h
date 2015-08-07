#ifndef PLOTVIEWER_H
#define PLOTVIEWER_H

#include <QDialog>
#include "qcustomplot.h"
#include <armadillo>

namespace Ui {
class PlotViewer;
}

class PlotViewer : public QDialog
{
    Q_OBJECT

public:
    explicit PlotViewer(QWidget *parent, const arma::vec &x, const arma::vec &y, arma::vec z, QString x_label, QString y_label);
    PlotViewer(QWidget *parent, const arma::vec &x, const arma::vec &y, QString x_label, QString y_label);
    ~PlotViewer();

private:
    Ui::PlotViewer *ui;
    QCustomPlot *plot_;
    bool mappeed_;
};

#endif // PLOTVIEWER_H
