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
    matrix_selection_dialog_ = new MatrixSelectionDialog(this, workspace_->dataset_tree_model());
    connect(matrix_selection_dialog_, &MatrixSelectionDialog::MatrixSelected,
            this, &ClassicalLeastSquaresDialog::MatrixSelected);
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

void ClassicalLeastSquaresDialog::MatrixSelected(QStringList keys)
{
    matrix_keys_ = keys;
    ui->matrixLabel->setText(matrix_keys_.last());
    raise();
}

void ClassicalLeastSquaresDialog::on_buttonBox_accepted()
{
    if (!dataset_keys_.isEmpty() || !dataset_.isNull()){
        QString name = ui->nameLineEdit->text();
        if (!dataset_keys_.empty()){
            try{
                MultiAnalyzer analyzer(workspace_, dataset_keys_);
                analyzer.ClassicalLeastSquares(name, matrix_keys_);
            }catch(exception e){
                workspace_->main_window()->DisplayExceptionWarning(e);
            }
        }
        else{
            try{
                dataset_->ClassicalLeastSquares(name, matrix_keys_);
            }catch(exception e){
                workspace_->main_window()->DisplayExceptionWarning(e);
            }
        }
    }
    dataset_.clear();
    close();
}

void ClassicalLeastSquaresDialog::on_selectPushButton_clicked()
{
    matrix_selection_dialog_->show();
}
