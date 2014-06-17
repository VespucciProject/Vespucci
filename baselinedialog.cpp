#include "baselinedialog.h"
#include "ui_baselinedialog.h"

BaselineDialog::BaselineDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::BaselineDialog)
{
    ui->setupUi(this);
    workspace = ws;
    dataset_ = workspace->DatasetAt(row);

    window_size_box_ = this->findChild<QSpinBox *>("windowSpinBox");
    method_box_ = this->findChild<QComboBox *>("methodComboBox");
    iterations_box_ = this->findChild<QSpinBox *>("iterationsSpinBox");
}

BaselineDialog::~BaselineDialog()
{
    delete ui;
}

void BaselineDialog::on_buttonBox_accepted()
{
    int window_size = window_size_box_->value();
    QString method = method_box_->currentText();
    int iterations = iterations_box_->value();

    for (int i = 0; i < iterations; ++i)
        dataset_->Baseline(method, window_size);
}
