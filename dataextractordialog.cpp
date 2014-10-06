#include "dataextractordialog.h"
#include "ui_dataextractordialog.h"

///
/// \brief DataExtractorDialog::DataExtractorDialog
/// \param parent Parent QWidget
/// \param map The map from which the new dataset is created
/// \param dataset The current dataset from which the new dataset is created
/// \param main_window The main window of the program.
///
DataExtractorDialog::DataExtractorDialog(QWidget *parent, MapData *map, QSharedPointer<VespucciDataset> dataset, MainWindow *main_window) :
    QDialog(parent),
    ui(new Ui::DataExtractorDialog)
{
    ui->setupUi(this);
    main_window_ = main_window;
    workspace = main_window->workspace_ptr();
    name_line_edit_ = this->findChild<QLineEdit*>("nameLineEdit");
    upper_box_ = this->findChild<QDoubleSpinBox*>("upperDoubleSpinBox");
    lower_box_ = this->findChild<QDoubleSpinBox*>("lowerDoubleSpinBox");
    map_ = map;
    dataset_ = dataset;
    data_range_ = map->dataRange();
    upper_box_->setValue(data_range_.upper);
    lower_box_->setValue(data_range_.lower);
    upper_box_->setRange(data_range_.lower, data_range_.upper);
    lower_box_->setRange(data_range_.lower, data_range_.upper);
    name_line_edit_->setText(dataset_->name() + "-" + map_->name());
}

DataExtractorDialog::~DataExtractorDialog()
{
    delete ui;
}

///
/// \brief DataExtractorDialog::on_buttonBox_accepted
/// Calls constructor for new dataset object when user clicks "Ok"
void DataExtractorDialog::on_buttonBox_accepted()
{
    QString name = name_line_edit_->text();
    uvec indices = map_->extract_range(lower_box_->value(), upper_box_->value());
    QFile *log_file = workspace->CreateLogFile(name);

    QSharedPointer<VespucciDataset> new_dataset(new VespucciDataset(name,
                                                    main_window_, 
                                                    workspace->directory_ptr(),
                                                    log_file,
                                                    dataset_, 
                                                    indices));
    workspace->AddDataset(new_dataset);
    this->close();
}

///
/// \brief DataExtractorDialog::on_buttonBox_rejected
/// Closes the window when the user selects "Cancel"
void DataExtractorDialog::on_buttonBox_rejected()
{
    this->close();
}
