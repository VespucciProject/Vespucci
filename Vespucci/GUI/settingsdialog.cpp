#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    workspace_ = ws;

    settings_ = workspace_->settings();
    settings_->beginGroup("environment");
    QString r_home = settings_->value("R_HOME").toString();
    settings_->endGroup();
    settings_->beginGroup("specdata");
    QString abs_label = settings_->value("absLabel").toString();
    QString abs_units = settings_->value("absUnits").toString();
    QString ord_label = settings_->value("ordLabel").toString();
    QString ord_units = settings_->value("ordUnits").toString();
    settings_->endGroup();

    ui->rHomeLineEdit->setText(r_home);
    ui->absLabelLineEdit->setText(abs_label);
    ui->absUnitsLineEdit->setText(abs_units);
    ui->ordLabelLineEdit->setText(ord_label);
    ui->ordUnitsLineEdit->setText(ord_units);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    QString r_home = ui->rHomeLineEdit->text();
    QString abs_label = ui->absLabelLineEdit->text();
    QString abs_units = ui->absUnitsLineEdit->text();
    QString ord_label = ui->ordLabelLineEdit->text();
    QString ord_units = ui->ordUnitsLineEdit->text();

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
