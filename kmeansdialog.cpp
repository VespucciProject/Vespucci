#include "kmeansdialog.h"
#include "ui_kmeansdialog.h"

KMeansDialog::KMeansDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::KMeansDialog)
{
    ui->setupUi(this);
    name_line_edit_ = this->findChild<QLineEdit *>("nameLineEdit");
    cluster_spin_box_ = this->findChild<QSpinBox *>("clustersSpinBox");
    workspace = ws;
    data_ = workspace->DatasetAt(row);
    data_index_ = row;
}

KMeansDialog::~KMeansDialog()
{
    delete ui;
}

void KMeansDialog::on_buttonBox_accepted()
{
    int clusters = cluster_spin_box_->value();
    QString name = name_line_edit_->text();
    data_->KMeans(clusters, name);
}
