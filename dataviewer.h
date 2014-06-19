#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QDialog>
#include "vespucciworkspace.h"
namespace Ui {
class DataViewer;
}

class DataViewer : public QDialog
{
    Q_OBJECT

public:
    explicit DataViewer(QWidget *parent = 0, VespucciWorkspace *ws = 0, int row = 0);
    ~DataViewer();

private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::DataViewer *ui;
    QSharedPointer<SpecMap> dataset_;
    VespucciWorkspace *workspace;
    QTableWidget *table_;
    QComboBox *data_selector_;
    bool constructor_in_progress_;

};

#endif // DATAVIEWER_H
