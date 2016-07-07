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
MapViewer::MapViewer(QString name, QString *directory, MapData *parent):
    QMainWindow(0),
    ui(new Ui::MapViewer)
{
    ui->setupUi(this);
    name_ = name;
    directory_ = directory;

    ui->mapView->setBackground(palette().window());

    parent_map_data_ = parent;
    //color_map_ = qobject_cast<QCPColorMap *>(color_map_abs);


    color_list_ = QStringList({"ColorBrewerBlueGreen",
                               "ColorBrewerBluePurple",
                               "ColorBrewerGreenBlue",
                               "ColorBrewerOrangeRed",
                               "ColorBrewerPurpleBlue",
                               "ColorBrewerPurpleBlueGreen",
                               "ColorBrewerPurpleRed",
                               "ColorBrewerRedPurple",
                               "ColorBrewerYellowGreen",
                               "ColorBrewerYellowGreenBlue",
                               "ColorBrewerYellowOrangeBrown",
                               "ColorBrewerYellowOrangeRed",
                               "ColorBrewerBlues",
                               "ColorBrewerGreens",
                               "ColorBrewerOranges",
                               "ColorBrewerPurples",
                               "ColorBrewerReds",
                               "ColorBrewerGrayscale",
                               "QCustomPlotGrayscale",
                               "QCustomPlotNight",
                               "QCustomPlotCandy",
                               "QCustomPlotIon",
                               "QCustomPlotThermal",
                               "↔QCustomPlotPolar",
                               "↔QCustomPlotSpectrum",
                               "QCustomPlotJet",
                               "QCustomPlotHues",
                               "QCustomPlotHot",
                               "QCustomPlotCold",
                               "↔ColorBrewerBrownBlueGreen",
                               "↔ColorBrewerPinkYellowGreen",
                               "↔ColorBrewerPurpleGreen",
                               "↔ColorBrewerPurpleOrange",
                               "↔ColorBrewerRedBlue",
                               "↔ColorBrewerRedGray",
                               "↔ColorBrewerRedYellowBlue",
                               "↔ColorBrewerRedYellowGreen",
                               "↔ColorBrewerSpectral",
                               "↔VespucciSpectral"});
}

MapViewer::~MapViewer()
{
    delete ui;
}

///
/// \brief MapViewer::GetGradient
/// \param gradient_number index of color gradient
/// \return Color gradient
///
QCPColorGradient MapViewer::GetGradient(int gradient_number)
{
    switch (gradient_number)
    {
    case 0: return QCPColorGradient::cbBuGn;
    case 1: return QCPColorGradient::cbBuPu;
    case 2: return QCPColorGradient::cbGnBu;
    case 3: return QCPColorGradient::cbOrRd;
    case 4: return QCPColorGradient::cbPuBu;
    case 5: return QCPColorGradient::cbPuBuGn;
    case 6: return QCPColorGradient::cbPuRd;
    case 7: return QCPColorGradient::cbRdPu;
    case 8: return QCPColorGradient::cbYlGn;
    case 9: return QCPColorGradient::cbYlGnBu;
    case 10: return QCPColorGradient::cbYlOrBr;
    case 11: return QCPColorGradient::cbYlOrRd;
    case 12: return QCPColorGradient::cbBlues;
    case 13: return QCPColorGradient::cbGreens;
    case 14: return QCPColorGradient::cbOranges;
    case 15: return QCPColorGradient::cbPurples;
    case 16: return QCPColorGradient::cbReds;
    case 17: return QCPColorGradient::cbGreys;
    case 18: return QCPColorGradient::gpGrayscale;
    case 19: return QCPColorGradient::gpNight;
    case 20: return QCPColorGradient::gpCandy;
    case 21: return QCPColorGradient::gpIon;
    case 22: return QCPColorGradient::gpThermal;
    case 23: return QCPColorGradient::gpPolar;
    case 24: return QCPColorGradient::gpSpectrum;
    case 25: return QCPColorGradient::gpJet;
    case 26: return QCPColorGradient::gpHues;
    case 27: return QCPColorGradient::gpHot;
    case 28: return QCPColorGradient::gpCold;
    case 29: return QCPColorGradient::cbBrBG;
    case 30: return QCPColorGradient::cbPiYG;
    case 31: return QCPColorGradient::cbPRGn;
    case 32: return QCPColorGradient::cbPuOr;
    case 33: return QCPColorGradient::cbRdBu;
    case 34: return QCPColorGradient::cbRdGy;
    case 35: return QCPColorGradient::cbRdYlBu;
    case 36: return QCPColorGradient::cbRdYlGn;
    case 37: return QCPColorGradient::cbSpectral;
    case 38: return QCPColorGradient::vSpectral;
    default: return QCPColorGradient::gpCold;
    }
}


///
/// \brief MapViewer::on_actionInterpolate_triggered
/// see MapViewer::on_actionInterpolate_toggled
void MapViewer::on_actionInterpolate_triggered()
{
    //see on_actionInterpolate_toggled
}

///
/// \brief MapViewer::on_actionInterpolate_toggled
/// \param arg1
/// Turns interpolation of the QCustomPlot object on and off
void MapViewer::on_actionInterpolate_toggled(bool arg1)
{
    parent_map_data_->setInterpolate(arg1);
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
    Vespucci::SavePlot(ui->mapView, filename);

    /*
    QStringList filename_list = filename.split(".");
    QString extension = filename_list.last();

    //this method of determining type may not be valid on non-Win platforms
    //check this on GNU/Linux and Mac OSX later.




    if (extension == "bmp")
        Vespucci::SavePlot(ui->mapView, filename);
        parent_map_data_->saveBmp(filename, 0, 0, 1.0);
    else if (extension == "pdf")
        parent_map_data_->savePdf(filename, 0, 0);
    else if (extension == "png"){
        bool ok;
        int quality = QInputDialog::getInt(this, "Enter Quality",
                                           "Quality (%)",
                                           80, 0, 100, 1, &ok);
        if (ok)
            parent_map_data_->savePng(filename, 0, 0, 1.0, quality);
    }

    else if (extension == "jpg"){
        bool ok;
        int quality = QInputDialog::getInt(this, "Enter Quality",
                                           "Quality (%)",
                                           80, 0, 100, 1, &ok);
        if (ok)
            parent_map_data_->saveJpg(filename, 0, 0, 1.0, quality);
    }

    else{
        //default to tif, force extension (for Windows compatability)
        if (extension != "tif")
            filename.append(".tif");
        bool ok;
        int quality = QInputDialog::getInt(this,
                                           "Compression",
                                           "Enter 0 for no compression,"
                                           "1 for LZW lossless compression",
                                           0, 0, 1, 1, &ok);
        if (ok)
            parent_map_data_->saveTiff(filename, 0, 0, 1.0, quality);
    }
    */

}

///
/// \brief MapViewer::on_actionShow_Axes_toggled
/// \param arg1 Whether or not the Show Axes option is selected
/// Toggles whether or not the axes of the map are visible
void MapViewer::on_actionShow_Axes_toggled(bool arg1)
{
    parent_map_data_->ShowAxes(arg1);
}

///
/// \brief MapViewer::on_actionShow_Color_Scale_toggled
/// \param arg1 Whether or not the Show Color Scale option is selected
/// toggles whether or not the color scale is visible
void MapViewer::on_actionShow_Color_Scale_toggled(bool arg1)
{
    parent_map_data_->ShowColorScale(arg1);
}

///
/// \brief MapViewer::on_actionSet_Color_Scheme_triggered
/// Opens a dialog to select a new color scheme
void MapViewer::on_actionSet_Color_Scheme_triggered()
{
    QString color_name = QInputDialog::getItem(this, "Select Color Scheme", "Choose Scheme", color_list_);
    int color_index = color_list_.indexOf(color_name);
    QCPColorGradient new_gradient = GetGradient(color_index);
    parent_map_data_->setGradient(new_gradient);
}

///
/// \brief MapViewer::on_actionAdd_Scale_Bar_triggered
/// Launches a ScaleBarDialog to create a scale bar on the map
void MapViewer::on_actionAdd_Scale_Bar_triggered()
{
    //widget will delete itself
    ScaleBarDialog *scale_bar_dialog = new ScaleBarDialog(this, parent_map_data_);
    scale_bar_dialog->show();
}

///
/// \brief MapViewer::on_actionShow_Spectrum_Viewer_triggered
/// Makes the spectrum viewer visible
void MapViewer::on_actionShow_Spectrum_Viewer_triggered()
{
    parent_map_data_->ShowSpectrumViewer(true);
}

///
/// \brief MapViewer::on_actionCommon_Color_Gradient_toggled
/// \param arg1 Whether or not Common Color Gradient option is selected.
/// Sets the color scale to the global color scale.
void MapViewer::on_actionCommon_Color_Gradient_toggled(bool arg1)
{
    parent_map_data_->UseGlobalColorScale(arg1);
}

///
/// \brief MapViewer::GlobalDataRangeChanged
/// \param new_range The new global color range
/// Sets the range to the global color range. Associated with a signal in MainWindow
void MapViewer::GlobalDataRangeChanged(QCPRange new_range)
{
    parent_map_data_->SetDataRange(new_range);
}

///
/// \brief MapViewer::GlobalGradientChanged
/// \param new_gradient
/// Sets the new global color gradient when the global gradient is changed
void MapViewer::GlobalGradientChanged(QCPColorGradient new_gradient)
{
    parent_map_data_->setGradient(new_gradient);
}

///
/// \brief MapViewer::on_actionLock_Size_toggled
/// \param arg1 Whether or not the Lock Size option is selected
/// Locks the size of the MapDisplay window
void MapViewer::on_actionLock_Size_toggled(bool arg1)
{
    parent_map_data_->LockMapDisplaySize(arg1);
}

///
/// \brief MapViewer::on_actionReset_Size_triggered
/// Resets the size to its original.
void MapViewer::on_actionReset_Size_triggered()
{
    parent_map_data_->ResetMapWidgetSize();
}

///
/// \brief MapViewer::on_actionReproportion_triggered
/// Changes the size of the window so that it has the same width/height ratio as
/// it had on instantiation.
void MapViewer::on_actionReproportion_triggered()
{
    parent_map_data_->RescaleMapWidget();
}

///
/// \brief MapViewer::on_actionNew_Dataset_from_Map_triggered
/// Launches the data extractor from the MapData object.
void MapViewer::on_actionNew_Dataset_from_Map_triggered()
{
    parent_map_data_->LaunchDataExtractor();
}

void MapViewer::closeEvent(QCloseEvent *event)
{
    if (parent_map_data_->SpectrumViewerVisible())
        parent_map_data_->HideSpectrumViewer();
    event->accept();
}

void MapViewer::on_actionStats_triggered()
{
}

void MapViewer::on_actionExport_Values_triggered()
{
    parent_map_data_->ExportText();
}


void MapViewer::on_actionSet_Font_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,
                                      QFont("Arial", 12, QFont::Normal),
                                      this, "Select Font");
    if (ok){
        parent_map_data_->SetFonts(font);

    }


}
