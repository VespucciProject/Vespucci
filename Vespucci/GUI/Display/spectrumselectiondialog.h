#ifndef SPECTRUMSELECTIONDIALOG_H
#define SPECTRUMSELECTIONDIALOG_H

#include <QDialog>
#include "Data/Dataset/vespuccidataset.h"
#include "GUI/QAbstractItemModel/spectratablemodel.h"

class PlotViewer;
class MainWindow;
class SpectraTableModel;
class VespucciWorkspace;

namespace Ui {
class SpectrumSelectionDialog;
}

class SpectrumSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SpectrumSelectionDialog(MainWindow *main_window);
    ~SpectrumSelectionDialog();
signals:
    void SetActionChecked(bool checked);
protected:
    void showEvent(QShowEvent *ev);
    void closeEvent(QCloseEvent *ev);
public slots:
    void DatasetSelectionChanged(QString dataset_key);
    void DatasetToBeRemoved(QString key);
private slots:
    void on_tableView_clicked(const QModelIndex &index);
    void SpectrumRemoved(int row);
    void on_deletePushButton_clicked();
    void on_exportPushButton_clicked();

private:
    Ui::SpectrumSelectionDialog *ui;
    SpectraTableModel *table_model_;
    QSharedPointer<VespucciDataset> dataset_;
    PlotViewer *plot_viewer_;
    MainWindow *main_window_;
    QSharedPointer<VespucciWorkspace> workspace_;
};

#endif // SPECTRUMSELECTIONDIALOG_H
