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
#include "globalgradientdialog.h"
#include "ui_globalgradientdialog.h"
#include "Global/vespucciworkspace.h"

class MainWindow;

GlobalGradientDialog::GlobalGradientDialog(MainWindow *parent, QSharedPointer<VespucciWorkspace> workspace) :
    QDialog(parent),
    ui(new Ui::GlobalGradientDialog)
{
    ui->setupUi(this);
    workspace_ = workspace;
    color_range_dialog_ = new ColorRangeDialog(this);
    color_range_dialog_->SetGradientNames(workspace_->GradientNames());
    RefreshListWidget();
}

GlobalGradientDialog::~GlobalGradientDialog()
{
    delete ui;
}

void GlobalGradientDialog::AddGradient(QString name, QString gradient_key, double lower, double upper)
{
    if (name.isEmpty()) return;
    workspace_->AddGlobalGradient(name, gradient_key, lower, upper);
    RefreshListWidget();
}

void GlobalGradientDialog::on_addPushButton_clicked()
{
    QString basename = "Color Scale";
    QString name = basename;
    uint i = 0;
    while (workspace_->GradientNames().contains(name))
        name = basename + " (" + QString::number(i++) + ")";

    color_range_dialog_->SetRange(0, 100);
    color_range_dialog_->SetName(name, true);
    color_range_dialog_->show();
}

void GlobalGradientDialog::on_editPushButton_clicked()
{
    QString current_range_name = ui->gradientListWidget->currentItem()->text();
    QCPRange current_range = workspace_->GetGlobalGradient(current_range_name).range;
    color_range_dialog_->SetRange(current_range.lower, current_range.upper);
    color_range_dialog_->SetName(ui->gradientListWidget->currentItem()->text(), false);
    color_range_dialog_->show();
}

void GlobalGradientDialog::on_removePushButton_clicked()
{
    QString name = ui->gradientListWidget->currentItem()->text();
    workspace_->RemoveColorRange(name);
    RefreshListWidget();
}

void GlobalGradientDialog::on_recalculatePushButton_clicked()
{
    QString name = ui->gradientListWidget->currentItem()->text();
    workspace_->RecalculateGlobalGradient(name);
}

void GlobalGradientDialog::RefreshListWidget()
{
    ui->gradientListWidget->clear();
    ui->gradientListWidget->addItems(workspace_->GlobalGradientKeys());
}
