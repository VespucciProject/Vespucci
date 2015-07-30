#include "Global/vespucci.h"
#include <QtSvg>
#include <minizip/zip.h>
#include <cstdlib>
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
/// \brief Vespucci::SaveZipped
/// \param objects
/// \param filename
/// \param type
/// \return
///
bool Vespucci::SaveZipped(std::map<std::string, const arma::mat *> objects, const std::string filename, const arma::file_type type)
{
    std::vector<std::wstring> paths;
    for (auto& object : objects){
        try{
            object.second->save(object->first, type);
            paths.push_back(object.first);
        }catch(std::exception e){
            throw e;
        }
    }
    //copy to zip file
    Vespucci::CreateZipFile(filename, paths);

    //delete the temporary files
    for (const std::wstring &str : paths){remove(str.c_str());}


}



///
/// \brief Vespucci::CreateZipFile Move files to a zip file.
/// \param paths Vector of paths to add to file
/// \return
/// Credit to StackOverflow user niemiro
/// http://stackoverflow.com/questions/11370908/how-do-i-use-minizip-on-zlib
int Vespucci::CreateZipFile(std::string zip_path, std::vector<std::wstring> paths)
{
    zipFile zf = zipOpen(zip_path.c_str(), APPEND_STATUS_CREATE);
        if (zf == NULL)
            return 1;

        bool _return = true;
        for (size_t i = 0; i < paths.size(); i++)
        {
            std::fstream file(paths[i].c_str(), std::ios::binary | std::ios::in);
            if (file.is_open())
            {
                file.seekg(0, std::ios::end);
                long size = file.tellg();
                file.seekg(0, std::ios::beg);

                std::vector<char> buffer(size);
                if (size == 0 || file.read(&buffer[0], size))
                {
                    zip_fileinfo zfi = { 0 };
                    std::wstring fileName = paths[i].substr(paths[i].rfind('\\')+1);

                    if (S_OK == zipOpenNewFileInZip(zf, std::string(fileName.begin(), fileName.end()).c_str(), &zfi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION))
                    {
                        if (zipWriteInFileInZip(zf, size == 0 ? "" : &buffer[0], size))
                            _return = false;

                        if (zipCloseFileInZip(zf))
                            _return = false;

                        file.close();
                        continue;
                    }
                }
                file.close();
            }
            _return = false;
        }

        if (zipClose(zf, NULL))
            return 3;

        if (!_return)
            return 4;
        return S_OK;
}

///
/// \brief Vespucci::SaveVespucciBinary
/// \param spectra
/// \param x
/// \param y
/// \param abscissa
/// \return
///
bool Vespucci::SaveVespucciBinary(const arma::mat &spectra, const arma::vec &x, const arma::vec &y, const arma::vec &abscissa)
{
    try{
        arma::field<mat> dataset(4);
        dataset(0) = spectra_;
        dataset(1) = wavelength_;
        dataset(2) = x_;
        dataset(3) = y_;
        success = dataset.save(filename.toStdString(), arma_binary);
    }
    catch(exception e){
        cerr << "See armadillo exception" << endl;

        char str[50];
        strcat(str, "VespucciDataset::Save: ");
        strcat(str, e.what());
        throw std::runtime_error(str);
    }
}


bool Vespucci::SaveZipped(const arma::mat &spectra, const arma::vec &abscissa, const arma::vec &x, const arma::vec &y, const std::string filename, const arma::file_type type)
{
    std::map<std::string, const arma::mat*> map;
    map["x"] = &x;
    map["y"] = &y;
    map["spectra"] = &spectra;
    map["abscissa"] = &abscissa;
    SaveZipped(map, filename, file_type);
}
