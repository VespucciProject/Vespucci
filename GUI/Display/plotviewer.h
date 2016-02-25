#ifndef PLOTVIEWER_H
#define PLOTVIEWER_H

#include <QDialog>
#include "qcustomplot.h"
#include <mlpack/core.hpp>

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
    void SetLineStyle(QCPGraph::LineStyle ls);
    void SetDirectory(QString directory);
public slots:
    void ShowContextMenu(const QPoint & pos);
    void SaveTriggered();
private slots:

private:
    void AddContextMenuItems();
    Ui::PlotViewer *ui;
    QCustomPlot *custom_plot_;
    bool mappeed_;
    QMenu context_menu_;
    QString directory_;

};

#endif // PLOTVIEWER_H
