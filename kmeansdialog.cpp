#include "kmeansdialog.h"
#include "ui_kmeansdialog.h"

///
/// \brief KMeansDialog::KMeansDialog
/// \param parent Parent QWidget
/// \param ws Current workspace
/// \param row Row of current dataset
///
KMeansDialog::KMeansDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::KMeansDialog)
{
    ui->setupUi(this);
    name_line_edit_ = this->findChild<QLineEdit *>("nameLineEdit");
    cluster_spin_box_ = this->findChild<QSpinBox *>("clustersSpinBox");
    workspace = ws;
    data_ = workspace->DatasetAt(row);
    if(data_->non_spatial()){
        QMessageBox::warning(this,
                             "Non-spatial or Non-contiguous Dataset",
                             "Images cannot be created from non-spatial or "
                             "non-contiguous datasets.");
        this->close();
    }
    data_index_ = row;
}

KMeansDialog::~KMeansDialog()
{
    delete ui;
}

///
/// \brief KMeansDialog::on_buttonBox_accepted
/// Triggers K-means method of dataset when "Ok" selected
void KMeansDialog::on_buttonBox_accepted()
{
    int clusters = cluster_spin_box_->value();
    QString name = name_line_edit_->text();
    data_->KMeans(clusters, name);
}

///
/// \brief KMeansDialog::on_buttonBox_rejected
/// Closes window when "Cancel" selected.
void KMeansDialog::on_buttonBox_rejected()
{
    this->close();
}
