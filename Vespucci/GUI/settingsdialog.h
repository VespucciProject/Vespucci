#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <Global/vespucciworkspace.h>
namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws);
    ~SettingsDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SettingsDialog *ui;
    QSharedPointer<VespucciWorkspace> workspace_;
    QSettings *settings_;
};

#endif // SETTINGSDIALOG_H
