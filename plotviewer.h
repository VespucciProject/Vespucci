#ifndef PLOTVIEWER_H
#define PLOTVIEWER_H

#include <QDialog>

namespace Ui {
class PlotViewer;
}

///
/// \brief The PlotViewer class
/// For displaying plots that are not maps or spectra (PCA/VCA/PLS Scatterplots,
/// especially for meta datasets).
class PlotViewer : public QDialog
{
    Q_OBJECT

public:
    explicit PlotViewer(QWidget *parent = 0);
    ~PlotViewer();

private:
    Ui::PlotViewer *ui;
};

#endif // PLOTVIEWER_H
