#ifndef CLASSICALLEASTSQUARESDIALOG_H
#define CLASSICALLEASTSQUARESDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class ClassicalLeastSquaresDialog;
}

class ClassicalLeastSquaresDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClassicalLeastSquaresDialog(QWidget *parent, VespucciWorkspace *ws, int row);
    ~ClassicalLeastSquaresDialog();

private slots:
    void on_datasetRadioButton_clicked();

    void on_fileRadioButton_clicked();

    void on_buttonBox_accepted();

    void on_browsePushButton_clicked();

private:
    Ui::ClassicalLeastSquaresDialog *ui;
    QComboBox *dataset_combo_box_;
    QRadioButton *dataset_radio_button_;
    QRadioButton *file_radio_button_;
    QLineEdit *filename_line_edit_;
    QLineEdit *name_line_edit_;
    QPushButton *browse_push_button_;
    VespucciWorkspace *workspace;
    QSharedPointer<VespucciDataset> dataset_;
};

#endif // CLASSICALLEASTSQUARESDIALOG_H
