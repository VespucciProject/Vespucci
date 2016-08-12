#include "classicalleastsquaresdialog.h"
#include "ui_classicalleastsquaresdialog.h"
#include "Data/Analysis/multianalyzer.h"

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

///
/// \brief ClassicalLeastSquaresDialog::ClassicalLeastSquaresDialog
/// \param ws
/// \param dataset_keys
/// constructor from multianalysis
ClassicalLeastSquaresDialog::ClassicalLeastSquaresDialog(QSharedPointer<VespucciWorkspace> ws, const QStringList &dataset_keys)
    : QDialog(ws->main_window()),
      ui(new Ui::ClassicalLeastSquaresDialog)
{
    workspace_ = ws;
    dataset_keys_ = dataset_keys;
    if (dataset_keys_.isEmpty()) close();

}

ClassicalLeastSquaresDialog::~ClassicalLeastSquaresDialog()
{
    delete ui;
}



void ClassicalLeastSquaresDialog::on_buttonBox_accepted()
{
    if (dataset_keys_.isEmpty() && dataset_.isNull()){
        close();
        return;
    }
    QString name = ui->nameLineEdit->text();
    if (!dataset_keys_.empty()){
        try{
            QScopedPointer<MultiAnalyzer> analyzer(new MultiAnalyzer(workspace_, dataset_keys_));
            //analyzer->ClassicalLeastSquares(name, ui->referenceComboBox->currentText());
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
        }
        return;
    }
    try{
        dataset_->ClassicalLeastSquares(name, ui->referenceComboBox->currentText());
    }catch(exception e){
        workspace_->main_window()->DisplayExceptionWarning(e);
    }
}
