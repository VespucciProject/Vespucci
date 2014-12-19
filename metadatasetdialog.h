/*******************************************************************************
    Copyright (C) 2014 Wright State University - All Rights Reserved
    Daniel P. Foose - Author

    This file is part of Vespucci.

    Vespucci is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Vespucci is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Vespucci.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
#ifndef METADATASETDIALOG_H
#define METADATASETDIALOG_H

#include <QDialog>
#include "vespucciworkspace.h"

namespace Ui {
class MetaDatasetDialog;
}

class MetaDatasetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MetaDatasetDialog(QWidget *parent, VespucciWorkspace * ws);
    ~MetaDatasetDialog();

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::MetaDatasetDialog *ui;
    QListView *dataset_list_view_;
    QComboBox *method_selection_box_;
    QLineEdit *name_line_edit_;
    DatasetListModel *dataset_list_model_;
    VespucciWorkspace *workspace;
};

#endif // METADATASETDIALOG_H
