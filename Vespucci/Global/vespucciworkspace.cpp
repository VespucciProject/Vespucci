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
#include "Global/vespucciworkspace.h"
#include <iostream>
#include <QtSvg>
///
/// \brief VespucciWorkspace::VespucciWorkspace
/// Constructor
VespucciWorkspace::VespucciWorkspace() :
    settings_("settings.ini", QSettings::IniFormat)
{
    settings_.setIniCodec("UTF-8");
    dataset_loading_count_ = 0;
    directory_ = QDir::homePath();
    CheckSettings();
    data_model_ = new DataModel();
}

///
/// \brief VespucciWorkspace::~VespucciWorkspace
/// Destructor
VespucciWorkspace::~VespucciWorkspace()
{
    cout << "Workspace destructor" << endl;
    delete data_model_;
}

QStringList VespucciWorkspace::dataset_names() const
{
    return data_model_->DatasetNames();
}

///
/// \brief VespucciWorkspace::SetPointers
/// \param main_window
/// Set pointers to the main window in the workspace (mainwindow requires
/// workspace).
void VespucciWorkspace::SetPointers(MainWindow *main_window)
{
    main_window_ = main_window;
}


// FUNCTIONS THAT ADD AND REMOVE WORKSPACE OBJECTS

///
/// \brief VespucciWorkspace::AddDataset
/// \param dataset
/// Add a dataset to the datasets_ QList
void VespucciWorkspace::AddDataset(QSharedPointer<VespucciDataset> dataset)
{
    data_model_->AddDataset(dataset);
    dataset_tree_model_->SetupModelData(data_model_);
}

///
/// \brief VespucciWorkspace::RemoveDataset
/// \param name
/// We forget about this dataset. This does not guarantee that the memory is
/// deallocated, as the QSharedPointer may still belong to another active object
/// (we make sure to call QSharedPointer::clear at the ends of dialogs that
/// handle QSharedPointers for this reason).
void VespucciWorkspace::RemoveDataset(QString name)
{
    data_model_->RemoveDataset(name);
}

///
/// \brief VespucciWorkspace::UpdateModel
/// Call when a VespucciDataset adds or removes a new object.
void VespucciWorkspace::UpdateModel()
{
    dataset_tree_model_->SetupModelData(data_model_);
}



// Useful when the index is known (as when the list is being iterated)

///
/// \brief VespucciWorkspace::RemoveDatasetAt
/// \param i
/// Useful when the index is known (as when the list is being iterated)
/// These functions should be safe because the names list and the object list only update together

void VespucciWorkspace::RemoveDatasetAt(const QModelIndex &parent)
{
    //we need to know how deep this thing goes
    TreeItem *item = dataset_tree_model_->getItem(parent);
    QStringList keys = item->keys();
    if (keys.size() == 1) RemoveDataset(keys[0]);
    else return;
}

QSharedPointer<VespucciDataset> VespucciWorkspace::DatasetAt(const QModelIndex &parent)
{
    TreeItem *item = dataset_tree_model_->getItem(parent);
    QString dataset_key = item->keys()[0];
    return data_model_->GetDataset(dataset_key);
}




//ACCESS FUNCTIONS
///
/// \brief VespucciWorkspace::dataset_loading_count
/// \return
/// A count of the datasets that have been loaded.
int VespucciWorkspace::dataset_loading_count() const
{
    return dataset_loading_count_;
}

///
/// \brief VespucciWorkspace::directory
/// \return
/// The global working directory
QString VespucciWorkspace::directory() const
{
    return directory_;
}

///
/// \brief VespucciWorkspace::set_directory
/// \param directory New global directory
/// Sets the global directory
void VespucciWorkspace::set_directory(QString directory)
{
    directory_ = directory;
}

///
/// \brief VespucciWorkspace::main_window
/// \return Pointer to the main window
///
MainWindow *VespucciWorkspace::main_window()
{
    return main_window_;
}

///
/// \brief VespucciWorkspace::GetWavelengthMax
/// \param row Index of the dataset
/// \return Maximum of the spectral abscissa
///
double VespucciWorkspace::GetWavelengthMax(const QString &key) const
{
    double abscissa_max = 0;
    try{
        abscissa_max = data_model_->GetDataset(key)->abscissa_ptr()->max();
    }catch(exception e){
        main_window_->DisplayExceptionWarning("GetWavelengthMax", e);
    }

    return abscissa_max;

}

///
/// \brief VespucciWorkspace::GetDataset
/// \param key
/// \return
/// Will throw invalid argument (from DataModel) if key does not exist
QSharedPointer<VespucciDataset> VespucciWorkspace::GetDataset(const QString &key) const
{
    return data_model_->GetDataset(key);
}

///
/// \brief VespucciWorkspace::GetAnalysisResults
/// \param dataset_key
/// \param results_key
/// \return
/// Will throw invalid_argument (from DataModel) if key(s) do(es) not exist
QSharedPointer<AnalysisResults> VespucciWorkspace::GetAnalysisResults(const QString &dataset_key, const QString &results_key) const
{
    return data_model_->GetResults(dataset_key, results_key);
}

QSharedPointer<MapData> VespucciWorkspace::GetMap(const QString &dataset_key, const QString &map_key) const
{
    return data_model_->GetMap(dataset_key, map_key);
}

const mat &VespucciWorkspace::GetAuxiliaryMatrix(const QString &dataset_key, const QString &matrix_key) const
{
    return data_model_->GetAuxiliaryMatrix(dataset_key, matrix_key);
}

const mat &VespucciWorkspace::GetResultsMatrix(const QString &dataset_key, const QString &results_key, const QString &matrix_key) const
{
    return data_model_->GetResultsMatrix(dataset_key, results_key, matrix_key);
}

const mat &VespucciWorkspace::GetCoreMatrix(const QString &dataset_key, const QString &matrix_key) const
{
    return data_model_->GetCoreMatrix(dataset_key, matrix_key);
}

///
/// \brief VespucciWorkspace::GetWavelengthMin
/// \param row Index of the dataset
/// \return  Minimum of the spectral abscissa
///
double VespucciWorkspace::GetWavelengthMin(const QString &key) const
{
    double abscissa_min = 0;
    try{
        abscissa_min = data_model_->GetDataset(key)->abscissa_ptr()->min();
    }catch(exception e){
        main_window_->DisplayExceptionWarning("GetWavelengthMin", e);
    }
    return abscissa_min;
}

///
/// \brief VespucciWorkspace::directory_ptr
/// \return Pointer to global working directory
///
QString* VespucciWorkspace::directory_ptr()
{
    return &directory_;
}

///
/// \brief VespucciWorkspace::RecalculateGlobalDataRange
/// \param new_data_range Pointer to new data range
/// \return Whether or not data range was changed
///
bool VespucciWorkspace::RecalculateGlobalDataRange(QCPRange *new_data_range)
{
    if (new_data_range->upper > global_data_range_.upper){
        global_data_range_.upper = new_data_range->upper;
        return true;
    }
    if (new_data_range->lower < global_data_range_.lower){
        global_data_range_.lower = new_data_range->lower;
        return true;
    }
    return false;
}

///
/// \brief VespucciWorkspace::RefreshGlobalColorGradient
/// \param new_gradient New gradient
/// Changes the global gradient
void VespucciWorkspace::RefreshGlobalColorGradient(QCPColorGradient new_gradient)
{
    global_color_gradient_ = new_gradient;
}

///
/// \brief VespucciWorkspace::SetGlobalDataRange
/// \param new_data_range
/// Changes the global data range
void VespucciWorkspace::SetGlobalDataRange(QCPRange *new_data_range)
{
    global_data_range_ = *new_data_range;
}

///
/// \brief VespucciWorkspace::global_data_range
/// \return The global data range
///
QCPRange* VespucciWorkspace::global_data_range()
{
    return &global_data_range_;
}

///
/// \brief VespucciWorkspace::global_gradient
/// \return The global color gradient
///
QCPColorGradient* VespucciWorkspace::global_gradient()
{
    return &global_color_gradient_;
}

///
/// \brief VespucciWorkspace::CreateLogFile
/// \param dataset_name
/// \return
/// Creates a pointer to a heap-allocated QFile which is open.
QFile* VespucciWorkspace::CreateLogFile(QString dataset_name)
{
    //set up temporary filename (UTC date in close to ISO 8601 format)
    QDateTime datetime = QDateTime::currentDateTimeUtc();
    QString log_filename = datetime.toString("yyyy-MM-dd-hhmmss");
    log_filename += "_" + dataset_name + ".temp.txt";
    QFile *log_file = new QFile(log_filename);
    log_file->open(QIODevice::ReadWrite);
    return log_file;
}

///
/// \brief VespucciWorkspace::ClearDatasets
/// Clears the internal container for datasets in the list model
void VespucciWorkspace::ClearDatasets()
{
    dataset_tree_model_->ClearDatasets();
}


///
/// \brief VespucciWorkspace::SetListWidgetModel
/// \param model
/// Set the dataset list model
void VespucciWorkspace::SetListWidgetModel(DatasetTreeModel *model)
{
    dataset_tree_model_ = model;
}


/// \brief VespucciWorkspace::UpdateCount
/// \return The new count
/// Increments dataset loading count
unsigned int VespucciWorkspace::UpdateCount()
{
    return ++dataset_loading_count_;
}

///
/// \brief dataset_list_model
/// \return The dataset list model
///
DatasetTreeModel* VespucciWorkspace::dataset_tree_model() const
{
    return dataset_tree_model_;
}

void VespucciWorkspace::CleanLogFiles()
{
    QDir current_dir = QDir::current();
    QStringList filters;
    filters << "*.txt";
    foreach (QString filename, current_dir.entryList(filters, QDir::Files))
        QFile::remove(filename);
}

void VespucciWorkspace::ResetSettings()
{
    QString r_path;
#if defined Q_OS_WIN32
    r_path = "C:/Program Files/R/R-3.1.3";
#elif defined Q_OS_MAC
    r_path = "/Library/Frameworks/R.framework/Resources";
#else
    r_path = "/usr/lib/R";
#endif
    settings_.beginGroup("environment");
    settings_.setValue("R_HOME", r_path);
    settings_.endGroup();

    settings_.beginGroup("specdata");
    settings_.setValue("absLabel", "Raman Shift");
    settings_.setValue("absUnits", "cm⁻¹");
    settings_.setValue("ordLabel", "Intensity");
    settings_.setValue("ordUnits", "a.u.");
    settings_.endGroup();
}

void VespucciWorkspace::CheckSettings()
{
    QString r_path, r_home;
#if defined Q_OS_WIN32
    r_path = "C:/Program Files/R/R-3.1.3";
#elif defined Q_OS_MAC
    r_path = "/Library/Frameworks/R.framework/Resources";
#else
    r_path = "/usr/lib/R";
#endif

    if (!settings_.childGroups().contains("environment")){
        settings_.beginGroup("environment");
        settings_.setValue("R_HOME", r_path);
        settings_.endGroup();
    }
    else{
        settings_.beginGroup("environment");
        if (settings_.allKeys().contains("R_HOME")){
            r_home = settings_.value("R_HOME").toString();
        }
        else{
            r_home = r_path;
            settings_.setValue("R_HOME", r_path);
        }
        settings_.endGroup();
    }

    if (!settings_.childGroups().contains("specdata")){
        settings_.beginGroup("specdata");
        settings_.setValue("absLabel", "Raman Shift");
        settings_.setValue("absUnits", "cm⁻¹");
        settings_.setValue("ordLabel", "Intensity");
        settings_.setValue("ordUnits", "a.u.");
        settings_.endGroup();
    }
    else{
        settings_.beginGroup("specdata");
        if (!settings_.allKeys().contains("absLabel"))
            settings_.setValue("absLabel", "Raman Shift");
        if (!settings_.allKeys().contains("absUnits"))
            settings_.setValue("absUnits", "cm⁻¹");
        if (!settings_.allKeys().contains("ordLabel"))
            settings_.setValue("ordLabel", "Intensity");
        if (!settings_.allKeys().contains("ordUnits"))
            settings_.setValue("ordUnits", "a.u.");
        settings_.endGroup();
    }
}

QSettings *VespucciWorkspace::settings()
{
    return &settings_;
}

void VespucciWorkspace::UpdateTreeModel()
{
    dataset_tree_model_->UpdateData(data_model_);
}




/*
///
/// \brief VespucciWorkspace::SavePlot
/// \param plot
/// \param filename
/// \return
///
bool VespucciWorkspace::SavePlot(QCustomPlot *plot, QString filename) const
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
        plot->toPainter(&qcp_painter);


        QSvgGenerator generator;
        generator.setFileName(filename);

        QPainter painter;

        painter.begin(&generator);
        painter.drawPicture(0, 0, picture);
        painter.end();
    }
    else if (extension == "emf"){
        QStringList filename_trunk_list = filename_list;
        filename_trunk_list.removeLast();
        QString filename_trunk = filename_trunk_list.join(".");
        QString SVG_filename = filename_trunk + ".svg";
        QPicture picture;
        QCPPainter qcp_painter(&picture);
        plot->toPainter(&qcp_painter);

        QSvgGenerator generator;
        generator.setFileName(SVG_filename);

        QPainter painter;

        painter.begin(&generator);
        painter.drawPicture(0, 0, picture);
        painter.end();

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
*/
