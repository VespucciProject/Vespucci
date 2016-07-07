#ifndef PLOTVIEWER_H
#define PLOTVIEWER_H

#include <QDialog>
#include "qcustomplot.h"
#include <mlpack/core.hpp>
#include <GUI/Display/plotwidget.h>
#include <GUI/mainwindow.h>

namespace Ui {
class PlotViewer;
}
using namespace std;
using namespace arma;
class MainWindow;
class PlotViewer : public QDialog
{
    Q_OBJECT

public:
    explicit PlotViewer(MainWindow *parent);
    ~PlotViewer();
    void AddPlot(const mat& paired_data, const QString &tab_title);
    void AddPlot(const vec& abscissa, const vec& data, const QString &tab_title);
    void AddTransientPlot(const vec &abscissa, const vec &data, const QString &tab_title);
    void AddTransientPlot(const mat & paired_data, const QString &tab_title);
    void CloseTransientTab();
public slots:
private slots:

    void on_stackCheckBox_stateChanged(int arg1);

    void on_tabWidget_currentChanged(int index);

    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::PlotViewer *ui;
    QSharedPointer<VespucciWorkspace> workspace_;
};

#endif // PLOTVIEWER_H
