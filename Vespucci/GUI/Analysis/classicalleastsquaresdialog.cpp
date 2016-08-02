#include "classicalleastsquaresdialog.h"
#include "ui_classicalleastsquaresdialog.h"

ClassicalLeastSquaresDialog::ClassicalLeastSquaresDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::ClassicalLeastSquaresDialog)
{
    ui->setupUi(this);

    workspace_ = ws;
    dataset_ = workspace_->GetDataset(dataset_key);
    QStringList matrix_keys = dataset_->AuxiliaryMatrixKeys();
    ui->referenceComboBox->addItems(matrix_keys);

}

ClassicalLeastSquaresDialog::~ClassicalLeastSquaresDialog()
{
    delete ui;
}



void ClassicalLeastSquaresDialog::on_buttonBox_accepted()
{
    QString name = ui->nameLineEdit->text();
    try{
        dataset_->ClassicalLeastSquares(name, ui->referenceComboBox->currentText());
    }catch(exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
    }
}
