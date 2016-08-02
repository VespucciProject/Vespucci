#ifndef PLOTMAKERDIALOG_H
#define PLOTMAKERDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
#include "GUI/Display/plotviewer.h"

namespace Ui {
class PlotMakerDialog;
}

class PlotMakerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlotMakerDialog(MainWindow *parent,
                             PlotViewer *plot_viewer,
                             QSharedPointer<VespucciWorkspace> ws,
                             QStringList data_keys);
    ~PlotMakerDialog();

private slots:
    void on_buttonBox_accepted();


    void on_dimensionComboBox_currentTextChanged(const QString &arg1);

private:
    Ui::PlotMakerDialog *ui;
    QSharedPointer<VespucciWorkspace> workspace_;
    QStringList data_keys_;
    MainWindow *parent_;
    PlotViewer *plot_viewer_;
};

#endif // PLOTMAKERDIALOG_H
