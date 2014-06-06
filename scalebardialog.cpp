#include "scalebardialog.h"
#include "ui_scalebardialog.h"

ScaleBarDialog::ScaleBarDialog(QWidget *parent, MapData *map_data) :
    QDialog(parent),
    ui(new Ui::ScaleBarDialog)
{
    ui->setupUi(this);
    width_box_ = this->findChild<QDoubleSpinBox *>("widthDoubleSpinBox");
    height_box_ = this->findChild<QDoubleSpinBox *>("heightDoubleSpinBox");
    units_box_ = this->findChild<QLineEdit *>("unitsLineEdit");
    color_button_ = this->findChild<QPushButton *>("colorPushButton");
    position_box_ = this->findChild<QComboBox *>("positionComboBox");
    color_ = Qt::black;
    map_data_ = map_data;
}

ScaleBarDialog::~ScaleBarDialog()
{
    delete ui;
}

void ScaleBarDialog::on_colorPushButton_clicked()
{
    color_ = QColorDialog::getColor(color_, this, "Select Color");
}

void ScaleBarDialog::on_buttonBox_accepted()
{
    QMessageBox::information(this, "Debug", "call to DrawScaleBar");
    double width = width_box_->value();
    double height = height_box_->value();
    QString units = units_box_->text();
    QString position = position_box_->currentText();


    map_data_->DrawScaleBar(width,
                            height,
                            units,
                            color_,
                            position);
}
