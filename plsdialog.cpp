#include "plsdialog.h"
#include "ui_plsdialog.h"

PLSDialog::PLSDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::PLSDialog)
{
    ui->setupUi(this);
    workspace = ws;
    data_ = workspace->DatasetAt(row);
    if(data_->non_spatial()){
        QMessageBox::warning(this,
                             "Non-spatial or Non-contiguous Dataset",
                             "Images cannot be created from non-spatial or "
                             "non-contiguous datasets.");
        this->close();
    }

    image_component_selector_ = this->findChild<QSpinBox *>("componentSpinBox");
    components_selector_ = this->findChild<QSpinBox *>("componentsToCalculatespinBox");
    color_selector_ = this->findChild<QComboBox *>("gradientComboBox");
    recalculate_box_ = this->findChild<QCheckBox *>("recalculateCheckBox");
    name_box_ = this->findChild<QLineEdit*>("nameLineEdit");
    data_index_ = row;

}

PLSDialog::~PLSDialog()
{
    delete ui;
}

///
/// \brief PLSDialog::on_buttonBox_accepted
/// Call the appropriate function in VespucciDataset to perform PLS based on what
/// the user has entered when the user clicks "Ok"
void PLSDialog::on_buttonBox_accepted()
{
    int image_component = image_component_selector_->value();
    QString name = name_box_->text();
    bool recalculate = recalculate_box_->isChecked();
    int gradient_index = color_selector_->currentIndex();
    int components = components_selector_->value();
    data_->PartialLeastSquares(components, image_component, name, gradient_index, recalculate);
    this->close();
}

///
/// \brief PLSDialog::on_buttonBox_rejected
/// Close the window when the user clicks "Cancel"
void PLSDialog::on_buttonBox_rejected()
{
    this->close();
}
