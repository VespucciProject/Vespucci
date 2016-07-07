/*******************************************************************************
    Copyright (C) 2014-2016 Wright State University - All Rights Reserved
    Daniel P. Foose - Maintainer/Lead Developer

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
#ifndef LOADDATASET_H
#define LOADDATASET_H

#include <QDialog>
#include "Global/vespucciworkspace.h"

namespace Ui {
class LoadDataset;
}

///
/// \brief The LoadDataset class
/// Dialog that allows the user to import files into the program
class LoadDataset : public QDialog
{
    Q_OBJECT

public:
    explicit LoadDataset(QWidget *parent, QSharedPointer<VespucciWorkspace>  ws);
    ~LoadDataset();
public slots:
    void FilenameChanged(QString new_filename);
private slots:
    void on_browseButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_filenameLineEdit_textChanged(const QString &arg1);

private:
    Ui::LoadDataset *ui;
    ///
    /// \brief workspace
    /// The current workspace
    QSharedPointer<VespucciWorkspace> workspace_;

    ///
    /// \brief directory_
    /// The current working directory
    QString *directory_;
};

#endif // LOADDATASET_H
