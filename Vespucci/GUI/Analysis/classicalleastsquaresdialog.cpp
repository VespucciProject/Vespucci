#include "classicalleastsquaresdialog.h"
#include "ui_classicalleastsquaresdialog.h"

ClassicalLeastSquaresDialog::ClassicalLeastSquaresDialog(QWidget *parent, VespucciWorkspace *ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::ClassicalLeastSquaresDialog)
{
    ui->setupUi(this);
    reference_combo_box_ = findChild<QComboBox *>("datasetComboBox");
    name_line_edit_= findChild<QLineEdit *>("nameLineEdit");

    workspace = ws;
    dataset_ = workspace->GetDataset(dataset_key);
    QStringList matrix_keys = dataset_->AuxiliaryMatrixKeys();
    reference_combo_box_->addItems(matrix_keys);

}

ClassicalLeastSquaresDialog::~ClassicalLeastSquaresDialog()
{
    delete ui;
}



void ClassicalLeastSquaresDialog::on_buttonBox_accepted()
{
    QString name = name_line_edit_->text();
    try{
        dataset_->ClassicalLeastSquares(name, reference_combo_box_->currentText());
    }catch(exception e){
        workspace->main_window()->DisplayExceptionWarning(e);
    }
}
