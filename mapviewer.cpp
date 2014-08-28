/************************************************************************************
    Copyright (C) 2014 Daniel P. Foose - All Rights Reserved

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
***************************************************************************************/

#include "mapviewer.h"
#include "ui_mapviewer.h"


MapViewer::MapViewer(QString name, QString *directory, MapData *parent):
    QMainWindow(0),
    ui(new Ui::MapViewer)
{
    ui->setupUi(this);
    name_ = name;
    directory_ = directory;

    qcp_= this->findChild<QCustomPlot *>("mapView");
    qcp_->setBackground(this->palette().window());
    parent_ = parent;
    //color_map_ = qobject_cast<QCPColorMap *>(color_map_abs);


    color_list_ << "ColorBrewerBlueGreen"
                << "ColorBrewerBluePurple"
                << "ColorBrewerGreenBlue"
                << "ColorBrewerOrangeRed"
                << "ColorBrewerPurpleBlue"
                << "ColorBrewerPurpleBlueGreen"
                << "ColorBrewerPurpleRed"
                << "ColorBrewerRedPurple"
                << "ColorBrewerYellowGreen"
                << "ColorBrewerYellowGreenBlue"
                << "ColorBrewerYellowOrangeBrown"
                << "ColorBrewerYellowOrangeRed"
                << "ColorBrewerBlues"
                << "ColorBrewerGreens"
                << "ColorBrewerOranges"
                << "ColorBrewerPurples"
                << "ColorBrewerReds"
                << "ColorBrewerGrayscale"
                << "QCustomPlotGrayscale"
                << "QCustomPlotNight"
                << "QCustomPlotCandy"
                << "QCustomPlotIon"
                << "QCustomPlotThermal"
                << "↔QCustomPlotPolar"
                << "↔QCustomPlotSpectrum"
                << "QCustomPlotJet"
                << "QCustomPlotHues"
                << "QCustomPlotHot"
                << "QCustomPlotCold"
                << "↔ColorBrewerBrownBlueGreen"
                << "↔ColorBrewerPinkYellowGreen"
                << "↔ColorBrewerPurpleGreen"
                << "↔ColorBrewerPurpleOrange"
                << "↔ColorBrewerRedBlue"
                << "↔ColorBrewerRedGray"
                << "↔ColorBrewerRedYellowBlue"
                << "↔ColorBrewerRedYellowGreen"
                << "↔ColorBrewerSpectral"
                << "↔VespucciSpectral";
}

MapViewer::~MapViewer()
{
    delete ui;
}


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

void MapViewer::closeEvent(QCloseEvent * event)
{
    // the call to SpecMap::RemoveMapAt() results in program crashing

    int response = QMessageBox::question(this,
                                         "Close Map?",
                                         "Are you sure you want to close this"
                                         " map?  Map will be deleted.");
    if (response == QMessageBox::Yes){
        parent_->RemoveThis();
        event->accept();
    }
    else{
        event->ignore();
    }



    /*
    event->ignore();
    this->showMinimized();
    */
}

void MapViewer::on_actionInterpolate_triggered()
{
    //see on_actionInterpolate_toggled
}

void MapViewer::on_actionInterpolate_toggled(bool arg1)
{
    parent_->setInterpolate(arg1);
}


void MapViewer::on_actionSave_Image_As_triggered()
{
    QString path = *directory_;
    path = path + "/" + name_;

    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save File"),
                                         path,
                                         tr("Tagged Image File Format (*.tif);; "
                                            "Windows Bitmap (*.bmp);; "
                                            "Portable Network Graphics (*.png);; "
                                            "JPEG (*.jpg)"));

    QStringList filename_list = filename.split(".");
    QString extension = filename_list.last();

    //this method of determining type may not be valid on non-Win platforms
    //check this on GNU/Linux and Mac OSX later.

    if (extension == "bmp")
        parent_->saveBmp(filename, 0, 0, 1.0);

    else if (extension == "png"){
        bool ok;
        int quality = QInputDialog::getInt(this, "Enter Quality",
                                           "Quality (%)",
                                           80, 0, 100, 1, &ok);
        if (ok)
            parent_->savePng(filename, 0, 0, 1.0, quality);
    }

    else if (extension == "jpg"){
        bool ok;
        int quality = QInputDialog::getInt(this, "Enter Quality",
                                           "Quality (%)",
                                           80, 0, 100, 1, &ok);
        if (ok)
            parent_->saveJpg(filename, 0, 0, 1.0, quality);
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
            parent_->saveTiff(filename, 0, 0, 1.0, quality);
    }

}

void MapViewer::on_actionShow_Axes_toggled(bool arg1)
{
    parent_->ShowAxes(arg1);
}

void MapViewer::on_actionShow_Color_Scale_toggled(bool arg1)
{
    parent_->ShowColorScale(arg1);
}

void MapViewer::on_actionSet_Color_Scheme_triggered()
{
    QString color_name = QInputDialog::getItem(this, "Select Color Scheme", "Choose Scheme", color_list_);
    int color_index = color_list_.indexOf(color_name);
    QCPColorGradient new_gradient = GetGradient(color_index);
    parent_->setGradient(new_gradient);
}

void MapViewer::on_actionAdd_Scale_Bar_triggered()
{
    //widget will delete itself
    ScaleBarDialog *scale_bar_dialog = new ScaleBarDialog(this, parent_);
    scale_bar_dialog->show();
}


void MapViewer::on_actionShow_Spectrum_Viewer_triggered()
{
    parent_->ShowSpectrumViewer(true);
}

void MapViewer::on_actionCommon_Color_Gradient_toggled(bool arg1)
{
    cout << "MapViewer::on_actionCommon_Color_Gradient_toggled()" << endl;
    parent_->UseGlobalColorScale(arg1);
    cout << "end of function" << endl;
}

void MapViewer::GlobalDataRangeChanged(QCPRange new_range)
{
    parent_->SetDataRange(new_range);
}

void MapViewer::GlobalGradientChanged(QCPColorGradient new_gradient)
{
    parent_->setGradient(new_gradient);
}

void MapViewer::on_actionLock_Size_toggled(bool arg1)
{
    parent_->LockMapDisplaySize(arg1);
}

void MapViewer::on_actionReset_Size_triggered()
{
    parent_->ResetMapWidgetSize();
}

void MapViewer::on_actionReproportion_triggered()
{
    parent_->RescaleMapWidget();
}
