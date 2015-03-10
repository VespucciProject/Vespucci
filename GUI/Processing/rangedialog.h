#ifndef RANGEDIALOG_H
#define RANGEDIALOG_H

#include <QDialog>
#include "Data/Dataset/vespuccidataset.h"

namespace Ui {
class RangeDialog;
}
///
/// \brief The RangeDialog class
///
class RangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RangeDialog(QWidget *parent, double min, double max);
    ~RangeDialog();
    bool is_accepted();
    void GetRange(double &min, double &max);

signals:
    void DialogAccepted(double min, double max);


private slots:
    void on_buttonBox_accepted();

private:
    Ui::RangeDialog *ui;
    QDoubleSpinBox *min_box_;
    QDoubleSpinBox *max_box_;
    QLabel *range_label_;
    double min_;
    double max_;

    bool is_accepted_;
};

#endif // RANGEDIALOG_H
