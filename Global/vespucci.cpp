#include "Global/vespucci.h"
#include <QtSvg>
#include <minizip/zip.h>
#include <cstdlib>
#include <QProcess>
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
        int quality = QInputDialog::getInt(plot->parentWidget(), "Enter Quality",
                                           "Quality (%)",
                                           80, 0, 100, 1, &ok);
        if (ok)
            plot->savePng(filename, 0, 0, 1.0, quality);
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
        plot->replot();

        plot->toPainter(&qcp_painter);
        qcp_painter.end();

        QSvgGenerator generator;
        generator.setFileName(filename);

        QPainter painter;

        painter.begin(&generator);
        painter.drawPicture(0, 0, picture);
        painter.end();

        plot->setBackground(old_background);
        plot->replot();
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
        plot->replot();

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
            plot->replot();
            success = plot->saveRastered(filename, 0, 0, 1.0, "TIF", quality);
            plot->setBackground(background);
            plot->replot();

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


///
/// \brief Vespucci::SaveVespucciBinary
/// \param spectra
/// \param x
/// \param y
/// \param abscissa
/// \return
///
bool Vespucci::SaveVespucciBinary(std::string filename, const arma::mat &spectra, const arma::vec &x, const arma::vec &y, const arma::vec &abscissa)
{
    bool success;
    try{
        arma::field<arma::mat> dataset(4);
        dataset(0) = spectra;
        dataset(1) = abscissa;
        dataset(2) = x;
        dataset(3) = y;
        success = dataset.save(filename, arma::arma_binary);
    }
    catch(std::exception e){
        std::cerr << "See armadillo exception" << std::endl;
        std::string str = "Vespucci::SaveVespucciBinary: " + std::string(e.what());
        throw std::runtime_error(str);
    }
    return success;
}





bool Vespucci::SaveText(std::string basename, const arma::mat &spectra, const arma::vec &x, const arma::vec &y, const arma::vec &abscissa, arma::file_type type)
{
    std::string extension;
    switch (type){
    case arma::csv_ascii:
        extension = ".csv";
    case arma::raw_ascii: default:
        extension = ".txt";
    }
    bool x_success, y_success, spectra_success, abscissa_success;
    spectra_success = spectra.save(basename + "_spectra" + extension, type);
    abscissa_success = abscissa.save(basename + "_abscissa" + extension, type);
    x_success = x.save(basename + "_x" + extension, type);
    y_success = y.save(basename + "_y" + extension, type);
    return (x_success && y_success && spectra_success && abscissa_success);
}
