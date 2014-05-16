#include "principalcomponentsdialog.h"
#include "ui_principalcomponentsdialog.h"

PrincipalComponentsDialog::PrincipalComponentsDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::PrincipalComponentsDialog)
{
    ui->setupUi(this);
    workspace = ws;
    component_selector_ = this->findChild<QSpinBox *>("componentSpinBox");
    color_selector_ = this->findChild<QComboBox *>("gradientComboBox");
    name_box_ = this->findChild<QLineEdit*>("nameLineEdit");
    data_ = workspace->DatasetAt(row);
    data_index_ = row;
}

PrincipalComponentsDialog::~PrincipalComponentsDialog()
{
    delete ui;
}

void PrincipalComponentsDialog::on_buttonBox_accepted()
{
    int component = component_selector_->value();
    bool negative_scores = negative_score_box_->isChecked();
    QString name = name_box_->text();
    int gradient_index = color_selector_->currentIndex();
    data_->PrincipalComponents(component, negative_scores, name, gradient_index);
}
