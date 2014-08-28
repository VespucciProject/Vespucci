#include "vcadialog.h"
#include "ui_vcadialog.h"

VCADialog::VCADialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::VCADialog)
{
    cout << "VCADialog Constructor" << endl;
    ui->setupUi(this);
    workspace = ws;
    components_selector_ = this->findChild<QSpinBox *>("endmembersSpinBox");
    image_component_selector_ = this->findChild<QSpinBox *>("componentSpinBox");
    color_selector_ = this->findChild<QComboBox *>("gradientComboBox");
    recalculate_box_ = this->findChild<QCheckBox *>("recalculateCheckBox");
    negative_score_box_ = this->findChild<QCheckBox *>("negativeScoresCheckBox");
    name_box_ = this->findChild<QLineEdit*>("nameLineEdit");
    data_ = workspace->DatasetAt(row);
    data_index_ = row;
}

VCADialog::~VCADialog()
{
    delete ui;
}

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
                            true,
                            name,
                            gradient_index,
                            recalculate);
    this->close();
}

void VCADialog::on_buttonBox_rejected()
{
    this->close();
}
