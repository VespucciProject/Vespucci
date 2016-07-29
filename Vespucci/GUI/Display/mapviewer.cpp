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
#include "GUI/Display/mapviewer.h"
#include "ui_mapviewer.h"
#include "Global/vespucci.h"
#include "Global/global.h"

///
/// \brief MapViewer::MapViewer
/// \param name Name of this map
/// \param directory Working directory for this window
/// \param parent MapData associated with this image
/// Constructor for this object
MapViewer::MapViewer(MainWindow *parent, QSharedPointer<MapData> map_data, QSharedPointer<VespucciWorkspace> ws):
    QMainWindow(parent),
    ui(new Ui::MapViewer)
{
    ui->setupUi(this);
    workspace_ = ws;
    map_data_ = map_data;
    name_ = map_data->name();
    directory_ = ws->directory_ptr();
    ui->mapPlot->setBackground(palette().window());
    map_data->SetMapPlot(ui->mapPlot);
    ui->statusbar->addWidget(new QLabel("(0,0,0)"));
    connect(this, &MapViewer::RequestSpectrumPlot,
            parent, &MainWindow::SpectrumRequested);
    connect(this, &MapViewer::RequestHeldSpectrumPlot,
            parent, &MainWindow::HeldSpectrumRequested);
    connect(ui->mapPlot, &MapPlot::SpectrumRequested,
            this, &MapViewer::SpectrumRequested);
}

MapPlot *MapViewer::mapPlot()
{
    return ui->mapPlot;
}

MapViewer::~MapViewer()
{
    delete ui;
}

///
/// \brief MapViewer::SpectrumRequested
/// \param index
/// Passes signal on to main window
/// the widget doesn't know everything main window needs to know
void MapViewer::SpectrumRequested(size_t index)
{
    emit RequestSpectrumPlot(map_data_->keys().first(),
                             map_data_->keys().last(),
                             index);
}

///
/// \brief MapViewer::on_actionInterpolate_toggled
/// \param arg1
/// Turns interpolation of the QCustomPlot object on and off
void MapViewer::on_actionInterpolate_toggled(bool arg1)
{
    map_data_->setInterpolate(arg1);
}

///
/// \brief MapViewer::on_actionSave_Image_As_triggered
/// Save the current view of the QCustomPlot object
void MapViewer::on_actionSave_Image_As_triggered()
{
    QString path = *directory_;
    path = path + "/" + name_;

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save File"),
                                         path,
                                         tr("Scalable Vector Graphics (*.svg);; "
                                            "Enhanced Windows Metafile (*.emf);; "
                                            "Portable Document Format (*.pdf);; "
                                            "Tagged Image File Format (*.tif);; "
                                            "Windows Bitmap (*.bmp);; "
                                            "Portable Network Graphics (*.png);; "
                                            "JPEG (*.jpg)"));
    Vespucci::SavePlot(ui->mapPlot, filename);
}

///
/// \brief MapViewer::on_actionShow_Axes_toggled
/// \param arg1 Whether or not the Show Axes option is selected
/// Toggles whether or not the axes of the map are visible
void MapViewer::on_actionShow_Axes_toggled(bool arg1)
{
    map_data_->ShowAxes(arg1);
}

///
/// \brief MapViewer::on_actionShow_Color_Scale_toggled
/// \param arg1 Whether or not the Show Color Scale option is selected
/// toggles whether or not the color scale is visible
void MapViewer::on_actionShow_Color_Scale_toggled(bool arg1)
{
    map_data_->ShowColorScale(arg1);
}

///
/// \brief MapViewer::on_actionSet_Color_Scheme_triggered
/// Opens a dialog to select a new color scheme
void MapViewer::on_actionSet_Color_Scheme_triggered()
{
    if (!map_data_->global_gradient_key().isEmpty())
        QMessageBox::warning(this, "Global Gradient",
                             "Changing the color gradient will detach this map"
                             "from the global gradient");
    QStringList gradient_names = workspace_->GradientNames();
    QString gradient_key = QInputDialog::getItem(this, "Select Gradient", "Gradient",
                                                 gradient_names);
    QCPColorGradient new_gradient = workspace_->GetGradient(gradient_key);
    map_data_->setGradient(new_gradient);
}

///
/// \brief MapViewer::on_actionAdd_Scale_Bar_triggered
/// Launches a ScaleBarDialog to create a scale bar on the map
void MapViewer::on_actionAdd_Scale_Bar_triggered()
{
    //widget will delete itself
    ScaleBarDialog *scale_bar_dialog = new ScaleBarDialog(this, map_data_);
    scale_bar_dialog->setAttribute(Qt::WA_DeleteOnClose);
    scale_bar_dialog->show();
}


///
/// \brief MapViewer::on_actionLock_Size_toggled
/// \param arg1 Whether or not the Lock Size option is selected
/// Locks the size of the MapDisplay window
void MapViewer::on_actionLock_Size_toggled(bool arg1)
{
    map_data_->LockMapDisplaySize(arg1);
}

///
/// \brief MapViewer::on_actionReset_Size_triggered
/// Resets the size to its original.
void MapViewer::on_actionReset_Size_triggered()
{
    map_data_->ResetMapWidgetSize();
}

///
/// \brief MapViewer::on_actionReproportion_triggered
/// Changes the size of the window so that it has the same width/height ratio as
/// it had on instantiation.
void MapViewer::on_actionReproportion_triggered()
{
    map_data_->RescaleMapWidget();
}

void MapViewer::on_actionSet_Font_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,
                                      QFont("Arial", 12, QFont::Normal),
                                      this, "Select Font");
    if (ok) map_data_->SetFonts(font);
}

void MapViewer::on_actionSet_Color_Scale_Label_triggered()
{
    QString current_label = ui->mapPlot->ColorScaleLabel();
    bool ok = true;
    QString new_label = QInputDialog::getText(this, "Enter label",
                                              "Label",
                                              QLineEdit::Normal,
                                              current_label, &ok);
    if (ok) ui->mapPlot->SetColorScaleLabel(new_label);
}

void MapViewer::on_actionSet_Global_Color_Scale_triggered()
{
   QStringList global_scale_names = workspace_->GlobalGradientKeys();
   if (!global_scale_names.size()){
       QMessageBox::information(this, "No Global Gradients",
                                "No global gradients exist. To create a new"
                                "gradient, select Tools->Global Color Scales"
                                "in the main window");
       return;
   }
   bool ok;
   QString key = QInputDialog::getItem(this, "Select Global Gradient",
                                       "Gradient", global_scale_names,
                                       0, false, &ok);
   if (ok)
       map_data_->SetGlobalGradient(key);
}

void MapViewer::keyPressEvent(QKeyEvent *event)
{
    //arrow keys on some laptops might be difficult to use
    //so why not also accept WASD and HJKL?

    switch (event->key()) {
        case Qt::Key_Up:
        case Qt::Key_W:
        case Qt::Key_K:
            ui->mapPlot->MoveHorizontalCrosshair(1);
            return;
        case Qt::Key_Down:
        case Qt::Key_S:
        case Qt::Key_J:
            ui->mapPlot->MoveHorizontalCrosshair(-1);
            return;
        case Qt::Key_Left:
        case Qt::Key_A:
        case Qt::Key_H:
            ui->mapPlot->MoveVerticalCrosshair(-1);
            return;
        case Qt::Key_Right:
        case Qt::Key_F:
        case Qt::Key_L:
            ui->mapPlot->MoveVerticalCrosshair(1);
            return;
        case Qt::Key_Enter:
        case Qt::Key_Space:
            emit RequestHeldSpectrumPlot(map_data_->keys().first(),
                                         map_data_->keys().last(),
                                         ui->mapPlot->GetCrosshairPosition());
            return;
        default:
            return;
    }
}
