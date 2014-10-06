#include "vcadialog.h"
#include "ui_vcadialog.h"

///
/// \brief VCADialog::VCADialog
/// \param parent See QDialog
/// \param ws Current workspace
/// \param row Currently selected row in dataset list widget
///
VCADialog::VCADialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::VCADialog)
{
    cout << "VCADialog Constructor" << endl;
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
    components_selector_ = this->findChild<QSpinBox *>("endmembersSpinBox");
    image_component_selector_ = this->findChild<QSpinBox *>("componentSpinBox");
    color_selector_ = this->findChild<QComboBox *>("gradientComboBox");
    recalculate_box_ = this->findChild<QCheckBox *>("recalculateCheckBox");
    name_box_ = this->findChild<QLineEdit*>("nameLineEdit");
    data_index_ = row;
}

VCADialog::~VCADialog()
{
    delete ui;
}

///
/// \brief VCADialog::on_buttonBox_accepted
/// Calls appropriate dataset method when "Ok" is clicked.
void VCADialog::on_buttonBox_accepted()
{
    int image_component = image_component_selector_->value();
    int endmembers = components_selector_->value();
    if (image_component > endmembers)
        image_component = endmembers;

    QString name = name_box_->text();
    bool recalculate = recalculate_box_->isChecked();
    int gradient_index = color_selector_->currentIndex();
    data_->VertexComponents(endmembers,
                            image_component,
                            name,
                            gradient_index,
                            recalculate);
    this->close();
}

///
/// \brief VCADialog::on_buttonBox_rejected
/// Closes window when "Cancel" clicked.
void VCADialog::on_buttonBox_rejected()
{
    this->close();
}
