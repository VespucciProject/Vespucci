#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"
#include "specmap.h"

class VespucciWorkspace;
class SpecMap;
namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~FilterDialog();

private slots:


    void on_methodComboBox_currentIndexChanged(int index);

    void on_buttonBox_accepted();

private:
    Ui::FilterDialog *ui;
    VespucciWorkspace *workspace;
    QSharedPointer<SpecMap> dataset_;

    QComboBox *method_box_;
    QSpinBox *derivative_box_;
    QSpinBox *polynomial_box_;
    QSpinBox *window_box_;
};

#endif // FILTERDIALOG_H
