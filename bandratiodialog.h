#ifndef BANDRATIODIALOG_H
#define BANDRATIODIALOG_H

#include <QDialog>

namespace Ui {
class BandRatioDialog;
}

class BandRatioDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BandRatioDialog(QWidget *parent = 0);
    ~BandRatioDialog();

private:
    Ui::BandRatioDialog *ui;
};

#endif // BANDRATIODIALOG_H
