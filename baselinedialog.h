#ifndef BASELINEDIALOG_H
#define BASELINEDIALOG_H

#include <QDialog>

namespace Ui {
class BaselineDialog;
}

class BaselineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BaselineDialog(QWidget *parent = 0);
    ~BaselineDialog();

private:
    Ui::BaselineDialog *ui;
};

#endif // BASELINEDIALOG_H
