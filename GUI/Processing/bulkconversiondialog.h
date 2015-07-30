#ifndef BULKCONVERSIONDIALOG_H
#define BULKCONVERSIONDIALOG_H

#include <QDialog>

namespace Ui {
class BulkConversionDialog;
}

class BulkConversionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BulkConversionDialog(QWidget *parent = 0);
    ~BulkConversionDialog();

private:
    Ui::BulkConversionDialog *ui;
};

#endif // BULKCONVERSIONDIALOG_H
