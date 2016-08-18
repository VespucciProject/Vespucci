#ifndef BULKCONVERSIONDIALOG_H
#define BULKCONVERSIONDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
#include "GUI/mainwindow.h"
#include <mlpack/core.hpp>


namespace Ui {
class BulkConversionDialog;
}

class BulkConversionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BulkConversionDialog(MainWindow *parent, QSharedPointer<VespucciWorkspace> ws);
    ~BulkConversionDialog();

private slots:
    void on_browsePushButton_clicked();
    void DeleteItem();

    void on_buttonBox_accepted();

    void on_BrowsePushButton_clicked();

private:
    Ui::BulkConversionDialog *ui;
    QSharedPointer<VespucciWorkspace> workspace_;

    bool SaveHDF5(string filename,
                  const mat &spectra,
                  const vec &abscissa,
                  const vec &x,
                  const vec &y) const;
    enum infile_type{long_text, wide_text, binary, oldbinary};
    enum outfile_type{v_binary, text};
    vector<string> SaveFiles(vector<string> infile_names,
                             string outfile_path,
                             infile_type intype, arma::file_type outtype,
                             bool swap_spatial = false) const;

    const QString GetSep(const QString &filename) const;
    void WriteFile(const arma::file_type &type,
                   const string filename,
                   const arma::mat &spectra,
                   const arma::vec &abscissa,
                   const arma::vec &x, const arma::vec &y) const;

};

#endif // BULKCONVERSIONDIALOG_H
