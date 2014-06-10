#include "filterdialog.h"
#include "ui_filterdialog.h"

FilterDialog::FilterDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    workspace = ws;
    dataset_ = workspace->DatasetAt(row);
    method_box_ = this->findChild<QComboBox *>("methodComboBox");
    derivative_box_ = this->findChild<QSpinBox *>("derivativeSpinBox");
    polynomial_box_ = this->findChild<QSpinBox *>("polynomialSpinBox");
    window_box_ = this->findChild<QSpinBox *>("windowSpinBox");
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

///
/// \brief FilterDialog::on_methodComboBox_currentIndexChanged
/// \param index
///enables and disables appropriate options based on method selected
void FilterDialog::on_methodComboBox_currentIndexChanged(int index)
{
    if ((index!=2) && (index!=3) &&(derivative_box_->isEnabled()))
        derivative_box_->setEnabled(false);
    if ((index == 2 || index == 3) && (!derivative_box_->isEnabled()))
        derivative_box_->setEnabled(true);
    if ((index != 2) && (index != 3) && (polynomial_box_->isEnabled()))
        polynomial_box_->setEnabled(false);
    if ((index == 2 || index == 3)&&(!polynomial_box_->isEnabled()))
        polynomial_box_->setEnabled(true);
}

void FilterDialog::on_buttonBox_accepted()
{
    switch (method_box_->currentIndex())
    {
    case 0:
        dataset_->MedianFilter(window_box_->value());
        break;
    case 1:
        dataset_->LinearMovingAverage(window_box_->value());
        break;
    case 2:
        dataset_->Derivatize(derivative_box_->value(),
                             polynomial_box_->value(),
                             window_box_->value());
        break;
    case 3:
        dataset_->SavitzkyGolay(polynomial_box_->value(), window_box_->value());
        break;
    case 4:
        dataset_->SingularValue();
    default:
        return;
    }
}
