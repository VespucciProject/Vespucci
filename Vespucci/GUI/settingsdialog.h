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
    QLineEdit *r_home_line_edit_;
    QLineEdit *abs_label_line_edit_;
    QLineEdit *ord_label_line_edit_;
    QLineEdit *abs_units_line_edit_;
    QLineEdit *ord_units_line_edit_;

    QSharedPointer<VespucciWorkspace> workspace_;
    QSettings *settings_;
};

#endif // SETTINGSDIALOG_H
