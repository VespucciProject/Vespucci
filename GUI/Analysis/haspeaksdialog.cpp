#include "haspeaksdialog.h"
#include "ui_haspeaksdialog.h"

HasPeaksDialog::HasPeaksDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::HasPeaksDialog)
{
    ui->setupUi(this);
    dataset_ = ws->DatasetAt(row);

    table_widget_ = findChild<QTableWidget *>("tableWidget");
    min_box_ = findChild<QDoubleSpinBox *>("minDoubleSpinBox");
    max_box_ = findChild<QDoubleSpinBox *>("maxDoubleSpinBox");

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

}
