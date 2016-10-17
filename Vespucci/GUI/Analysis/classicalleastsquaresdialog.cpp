#include "classicalleastsquaresdialog.h"
#include "ui_classicalleastsquaresdialog.h"
#include "Data/Analysis/multianalyzer.h"

ClassicalLeastSquaresDialog::ClassicalLeastSquaresDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, QSharedPointer<AbstractDataAnalyzer> analyzer) :
    QDialog(parent),
    ui(new Ui::ClassicalLeastSquaresDialog),
    workspace_(ws),
    analyzer_(analyzer)
{
    ui->setupUi(this);
    matrix_selection_dialog_ = new MatrixSelectionDialog(this, workspace_->dataset_tree_model());
    connect(matrix_selection_dialog_, &MatrixSelectionDialog::MatrixSelected,
            this, &ClassicalLeastSquaresDialog::MatrixSelected);
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
    if (!analyzer_.isNull()){
        QString name = ui->nameLineEdit->text();
        try{
            analyzer_->ClassicalLeastSquares(name, matrix_keys_);
        }catch(exception e){
            workspace_->main_window()->DisplayExceptionWarning(e);
        }
    }
    analyzer_.clear();
    close();
}

void ClassicalLeastSquaresDialog::on_selectPushButton_clicked()
{
    matrix_selection_dialog_->show();
}
