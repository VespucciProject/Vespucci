#ifndef BULKCONVERSIONDIALOG_H
#define BULKCONVERSIONDIALOG_H

#include <QDialog>
#include "Global/vespucciworkspace.h"
#include "GUI/mainwindow.h"
#include <armadillo>


namespace Ui {
class BulkConversionDialog;
}

class BulkConversionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BulkConversionDialog(MainWindow *parent = 0, VespucciWorkspace *ws = 0);
    ~BulkConversionDialog();

private slots:
    void on_browsePushButton_clicked();
    void DeleteItem();

    void on_buttonBox_accepted();

private:
    Ui::BulkConversionDialog *ui;
    QListWidget *filename_list_widget_;
    QCheckBox *swap_box_;
    QComboBox *intype_box_;
    QComboBox *outtype_box_;
    QLineEdit *target_line_edit_;
    VespucciWorkspace *workspace;

    enum infile_type{long_text, wide_text, binary};
    enum outfile_type{v_binary, z_csv, z_txt};

    vector<string> SaveFiles(vector<string> infile_names,
                             string outfile_path,
                             infile_type intype, outfile_type outtype,
                             bool swap_spatial = false) const;

    const QString GetSep(const QString &filename) const;
    void WriteFile(const outfile_type & type,
                   const string filename,
                   const arma::mat &spectra,
                   const arma::vec &abscissa,
                   const arma::vec &x, const arma::vec &y) const;

};

#endif // BULKCONVERSIONDIALOG_H
