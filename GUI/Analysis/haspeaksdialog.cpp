#include "haspeaksdialog.h"
#include "ui_haspeaksdialog.h"

HasPeaksDialog::HasPeaksDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::HasPeaksDialog)
{

    ui->setupUi(this);
    dataset_ = ws->DatasetAt(row);
    workspace = ws;
    table_widget_ = findChild<QTableWidget *>("tableWidget");

    table_widget_->setRowCount(1);
    table_widget_->setColumnCount(2);
    QStringList header_labels = {QString::fromStdString("Minimum"), QString::fromStdString("Maximum")};
    table_widget_->setHorizontalHeaderLabels(header_labels);
}

HasPeaksDialog::~HasPeaksDialog()
{
    delete ui;
}

void HasPeaksDialog::on_buttonBox_accepted()
{
    if (table_widget_->rowCount() == 0){
        return;
    }
    mat points(table_widget_->rowCount(), 2);
    bool ok = true;
    double left, right;
    for (unsigned int i = 0; i < table_widget_->rowCount(); ++i){
        if (!table_widget_->item(i,0) || !table_widget_->item(i,1)){
            ok = false;
        }
        else{
            left = table_widget_->item(i, 0)->text().toDouble(&ok);
            right = table_widget_->item(i, 1)->text().toDouble(&ok);
        }

        if(!ok){
            QMessageBox::warning(this,
                                 "Parsing Error",
                                 "Row " +
                                 QString::number(i) +
                                 " skipped due to invalid text");
            points(i, 0) = 0;
            points(i, 1) = 0; //ranges with equal values are skipped
        }
        else{

            points(i, 0) = left;
            points(i, 1) = right;
        }
    }
    try{
        dataset_->HasPeaksCWT(points);
    }
    catch(exception e){
        workspace->main_window()->DisplayExceptionWarning(e);
    }
}

void HasPeaksDialog::on_addPushButton_clicked()
{

    int row = table_widget_->rowCount();
    table_widget_->insertRow(row);
}

void HasPeaksDialog::on_removePushButton_2_clicked()
{
    int row = table_widget_->currentRow();
    table_widget_->removeRow(row);
}
