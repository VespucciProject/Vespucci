#include "dataextractordialog.h"
#include "ui_dataextractordialog.h"

DataExtractorDialog::DataExtractorDialog(QWidget *parent, MapData *map, VespucciDataset *dataset, MainWindow *main_window) :
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

void DataExtractorDialog::on_buttonBox_accepted()
{

    uvec indices = map_->extract_range(lower_box_->value(), upper_box_->value());
    QSharedPointer<VespucciDataset> new_dataset(new VespucciDataset(name_line_edit_->text(), 
                                                    main_window_, 
                                                    workspace->directory_ptr(),
                                                    dataset_, 
                                                    indices));
    workspace->AddDataset(new_dataset);
}
