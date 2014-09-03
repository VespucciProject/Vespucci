#ifndef DATAEXTRACTORDIALOG_H
#define DATAEXTRACTORDIALOG_H

#include <QDialog>

namespace Ui {
class DataExtractorDialog;
}

class DataExtractorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataExtractorDialog(QWidget *parent = 0);
    ~DataExtractorDialog();

private:
    Ui::DataExtractorDialog *ui;
};

#endif // DATAEXTRACTORDIALOG_H
