#ifndef STATSDIALOG_H
#define STATSDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class StatsDialog;
}

class StatsDialog : public QDialog
{
    Q_OBJECT

public:
    StatsDialog(QWidget *parent, vec data);
    ~StatsDialog();


private slots:
    void on_buttonBox_accepted();

private:
    Ui::StatsDialog *ui;
    QLineEdit *min_line_edit_;
    QLineEdit *max_line_edit_;
    QLineEdit *median_line_edit_;
    QLineEdit *stddev_line_edit_;
    QLineEdit *mean_line_edit_;
};

#endif // STATSDIALOG_H
