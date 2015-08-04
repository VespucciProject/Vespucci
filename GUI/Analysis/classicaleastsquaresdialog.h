#ifndef CLASSICALEASTSQUARESDIALOG_H
#define CLASSICALEASTSQUARESDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class ClassicaLeastSquaresDialog;
}

class ClassicaLeastSquaresDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClassicaLeastSquaresDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~ClassicaLeastSquaresDialog();

private slots:
    void on_datasetRadioButton_clicked();

    void on_fileRadioButton_clicked();

    void on_imageCheckBox_stateChanged(int arg1);

    void on_buttonBox_accepted();

private:
    Ui::ClassicaLeastSquaresDialog *ui;
    QComboBox *dataset_combo_box_;
    QRadioButton *dataset_radio_button_;
    QRadioButton *file_radio_button_;
    QLineEdit *filename_line_edit_;
    QSpinBox *component_spin_box_;
    QComboBox *gradient_combo_box_;
    QCheckBox *image_check_box_;
    QLineEdit *name_line_edit_;
    QPushButton *browse_push_button_;
    VespucciWorkspace *workspace;
    QSharedPointer<VespucciDataset> dataset_;
};

#endif // CLASSICALEASTSQUARESDIALOG_H
