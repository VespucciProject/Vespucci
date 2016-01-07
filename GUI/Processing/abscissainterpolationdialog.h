#ifndef ABSCISSAINTERPOLATIONDIALOG_H
#define ABSCISSAINTERPOLATIONDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
namespace Ui {
class AbscissaInterpolationDialog;
}

class AbscissaInterpolationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AbscissaInterpolationDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~AbscissaInterpolationDialog();

private slots:
    void on_buttonBox_accepted();

    void on_sourceComboBox_currentIndexChanged(const QString &arg1);

    void on_methodComboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::AbscissaInterpolationDialog *ui;
    QComboBox *dataset_combo_box_;
    QRadioButton *dataset_radio_button_;
    QLineEdit *filename_line_edit_;
    QComboBox *method_combo_box_;
    QComboBox *source_combo_box_;
    QSpinBox *order_spin_box_;
    QSpinBox *window_spin_box_;
    QPushButton *browse_push_button_;

    QLabel *window_label_;
    QLabel *order_label_;
    QLabel *file_label_;
    QLabel *dataset_label_;

    QSharedPointer<VespucciDataset> dataset_;
    VespucciWorkspace *workspace;
};

#endif // ABSCISSAINTERPOLATIONDIALOG_H
