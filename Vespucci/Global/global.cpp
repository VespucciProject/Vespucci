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
#include "global.h"
#include <QtSvg>
///
/// \brief Vespucci::SavePlot
/// \param plot
/// \param filename
/// \return
///
bool Vespucci::SavePlot(QCustomPlot *plot, QString filename)
{
    QStringList filename_list = filename.split(".");
    QString extension = filename_list.last();

    //this method of determining type may not be valid on non-Win platforms
    //check this on GNU/Linux and Mac OSX later.

    bool success = true;

    if (extension == "bmp")
        success = plot->saveBmp(filename, 0, 0, 1.0);
    else if (extension == "pdf")
        success = plot->savePdf(filename, false, 0, 0, "Vespucci 1.0", "Plot");
    else if (extension == "png"){
        bool ok;
        QPixmap background = plot->background();
        plot->setBackground(Qt::transparent);
        plot->replot(QCustomPlot::rpImmediate);
        int quality = QInputDialog::getInt(plot->parentWidget(), "Enter Quality",
                                           "Quality (%)",
                                           80, 0, 100, 1, &ok);
        if (ok)
            plot->savePng(filename, 0, 0, 1.0, quality);
        plot->setBackground(background);
        plot->replot(QCustomPlot::rpImmediate);
    }

    else if (extension == "jpg"){
        bool ok;
        int quality = QInputDialog::getInt(plot->parentWidget(), "Enter Quality",
                                           "Quality (%)",
                                           80, 0, 100, 1, &ok);
        if (ok)
            plot->saveJpg(filename, 0, 0, 1.0, quality);
    }
    else if (extension == "svg"){
        QPicture picture;
        QCPPainter qcp_painter(&picture);
        qcp_painter.setMode(QCPPainter::pmVectorized);

        QPixmap old_background = plot->background();
        plot->setBackground(Qt::transparent);
        plot->replot(QCustomPlot::rpImmediate);

        plot->toPainter(&qcp_painter);
        qcp_painter.end();

        QSvgGenerator generator;
        generator.setFileName(filename);

        QPainter painter;

        painter.begin(&generator);
        painter.drawPicture(0, 0, picture);
        painter.end();

        plot->setBackground(old_background);
        plot->replot(QCustomPlot::rpImmediate);
    }
    else if (extension == "emf"){
        QStringList filename_trunk_list = filename_list;
        filename_trunk_list.removeLast();
        QString filename_trunk = filename_trunk_list.join(".");
        QString SVG_filename = filename_trunk + ".svg";

        QPicture picture;
        QCPPainter qcp_painter(&picture);
        qcp_painter.setMode(QCPPainter::pmVectorized);
        QPixmap old_background = plot->background();
        plot->setBackground(Qt::transparent);
        plot->replot(QCustomPlot::rpImmediate);

        plot->toPainter(&qcp_painter);
        qcp_painter.end();

        QSvgGenerator generator;
        generator.setFileName(SVG_filename);

        QPainter painter;

        painter.begin(&generator);
        painter.drawPicture(0, 0, picture);
        painter.end();

        plot->setBackground(old_background);
        plot->replot();

        //call java program "EMFGenerator" to convert svg file then
        QProcess *process = new QProcess(0);
        QString command = "java -jar EMFGenerator.jar \"" + SVG_filename + "\"";
        process->start(command);
    }

    else{
        //default to tif, force extension (for Windows compatability)
        if (extension != "tif")
            filename.append(".tif");
        bool ok;
        int quality = QInputDialog::getInt(plot->parentWidget(),
                                           "Compression",
                                           "Enter 0 for no compression,"
                                           "1 for LZW lossless compression",
                                           0, 0, 1, 1, &ok);
        if (ok){
            QPixmap background = plot->background();
            plot->setBackground(Qt::transparent);
            plot->replot(QCustomPlot::rpImmediate);
            success = plot->saveRastered(filename, 0, 0, 1.0, "TIF", quality);
            plot->setBackground(background);
            plot->replot(QCustomPlot::rpImmediate);

        }
    }

    return success;
}

///
/// \brief Vespucci::SetQCPFonts
/// \param plot
/// \param font
///
void Vespucci::SetQCPFonts(QCustomPlot *plot, const QFont &font)
{
    plot->setFont(font);

    plot->xAxis->setLabelFont(font);
    plot->xAxis->setTickLabelFont(font);

    plot->xAxis2->setLabelFont(font);
    plot->xAxis2->setTickLabelFont(font);

    plot->yAxis->setLabelFont(font);
    plot->yAxis->setTickLabelFont(font);

    plot->yAxis2->setLabelFont(font);
    plot->yAxis2->setTickLabelFont(font);

    plot->legend->setFont(font);
}


QProgressDialog *Vespucci::DisplayProgressDialog(QWidget *parent, QString title, QString text)
{
    QProgressDialog *progress = new QProgressDialog(parent);
    progress->setWindowModality(Qt::WindowModal);
    progress->setLabelText(text);
    progress->setWindowTitle(title);
    progress->setCancelButton(0);
    progress->setRange(0,0);
    progress->exec();
    return progress;
}

bool Vespucci::KeysAreEqual(QStringList &keys1, QStringList &keys2)
{
    if (keys1.size() != keys2.size()) return false;
    else{
        bool key_equal = false;
        for (int i = 0; i < keys1.size(); ++i){
            key_equal = (keys1[i] == keys2[i]);
        }
        return key_equal;
    }
}
