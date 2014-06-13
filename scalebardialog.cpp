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
    font_box_ = this->findChild<QFontComboBox *>("fontComboBox");
    font_size_box_ = this->findChild<QSpinBox *>("fontSizeSpinBox");
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
    QMessageBox::information(this, "Scale Bar", "Click on the map to show the scale bar.");
    double width = width_box_->value();
    double height = height_box_->value();
    QString units = units_box_->text();
    QString position = position_box_->currentText();
    QFont font = font_box_->currentFont();
    int font_size = font_size_box_->value();
    font.setPointSize(font_size);
    font.setBold(true);


    map_data_->DrawScaleBar(width,
                            height,
                            units,
                            color_,
                            position,
                            font);
}
