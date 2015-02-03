#ifndef PLOTMAKERDIALOG_H
#define PLOTMAKERDIALOG_H

#include <QDialog>

namespace Ui {
class PlotMakerDialog;
}

class PlotMakerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlotMakerDialog(QWidget *parent = 0);
    ~PlotMakerDialog();

private:
    Ui::PlotMakerDialog *ui;
};

#endif // PLOTMAKERDIALOG_H
