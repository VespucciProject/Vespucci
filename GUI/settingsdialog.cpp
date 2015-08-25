#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, VespucciWorkspace *ws) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    workspace = ws;
    r_home_line_edit_ = findChild<QLineEdit*>("rHomeLineEdit");
    abs_label_line_edit_ = findChild<QLineEdit*>("absLabelLineEdit");
    abs_units_line_edit_ = findChild<QLineEdit*>("absUnitsLineEdit");
    ord_label_line_edit_ = findChild<QLineEdit*>("ordLabelLineEdit");
    ord_units_line_edit_ = findChild<QLineEdit*>("ordUnitsLineEdit");

    settings_ = workspace->settings();

    QString r_home = settings_->value("R_HOME").toString();
    QString abs_label = settings_->value("absLabel").toString();
    QString abs_units = settings_->value("absUnit").toString();
    QString ord_label = settings_->value("ordLabel").toString();
    QString ord_units = settings_->value("ordUnits").toString();

    r_home_line_edit_->setText(r_home);
    abs_label_line_edit_->setText(abs_label);
    abs_units_line_edit_->setText(abs_units);
    ord_label_line_edit_->setText(ord_label);
    ord_units_line_edit_->setText(ord_units);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    QString r_home = r_home_line_edit_->text();
    QString abs_label = abs_label_line_edit_->text();
    QString abs_units = abs_units_line_edit_->text();
    QString ord_label = ord_label_line_edit_->text();
    QString ord_units = ord_units_line_edit_->text();

    settings_->beginGroup("environment");
    settings_->setValue("R_HOME", r_home);
    settings_->endGroup();
    settings_->beginGroup("specdata");
    settings_->setValue("absLabel", abs_label);
    settings_->setValue("absUnits", abs_units);
    settings_->setValue("ordLabel", ord_label);
    settings_->setValue("ordUnits", ord_units);
    settings_->endGroup();
}
