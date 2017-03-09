/*******************************************************************************
    Copyright (C) 2014-2017 Wright State University - All Rights Reserved
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
#include <H5Cpp.h>

///
/// \brief VespucciWorkspace::VespucciWorkspace
/// Constructor
VespucciWorkspace::VespucciWorkspace(QString settings_file) :
    settings_(settings_file, QSettings::IniFormat)
{
    settings_.setIniCodec("UTF-8");
    dataset_loading_count_ = 0;
    directory_ = QDir::homePath();
    CheckSettings();
    data_model_ = new DataModel();

    gradients_ = {
                    {"ColorBrewer BlueGreen", QCPColorGradient::cbBlues},
                    {"ColorBrewer BluePurple", QCPColorGradient::cbBuPu},
                    {"ColorBrewer GreenBlue", QCPColorGradient::cbGnBu},
                    {"ColorBrewer OrangeRed", QCPColorGradient::cbOrRd},
                    {"ColorBrewer PurpleBlue", QCPColorGradient::cbPuBu},
                    {"ColorBrewer PurpleBlueGreen", QCPColorGradient::cbPuBuGn},
                    {"ColorBrewer PurpleRed", QCPColorGradient::cbPuRd},
                    {"ColorBrewer RedPurple", QCPColorGradient::cbRdPu},
                    {"ColorBrewer YellowGreen", QCPColorGradient::cbYlGn},
                    {"ColorBrewer YellowGreenBlue", QCPColorGradient::cbYlGnBu},
                    {"ColorBrewer YellowOrangeBrown", QCPColorGradient::cbYlOrBr},
                    {"ColorBrewer YellowOrangeRed", QCPColorGradient::cbYlOrRd},
                    {"ColorBrewer Blues", QCPColorGradient::cbBlues},
                    {"ColorBrewer Greens", QCPColorGradient::cbGreens},
                    {"ColorBrewer Oranges", QCPColorGradient::cbOranges},
                    {"ColorBrewer Purples", QCPColorGradient::cbPurples},
                    {"ColorBrewer Reds", QCPColorGradient::cbReds},
                    {"ColorBrewer Grayscale", QCPColorGradient::cbGreys},
                    {"QCustomPlot Grayscale", QCPColorGradient::gpGrayscale},
                    {"QCustomPlot Night", QCPColorGradient::gpNight},
                    {"QCustomPlot Candy", QCPColorGradient::gpCandy},
                    {"QCustomPlot Ion", QCPColorGradient::gpIon},
                    {"QCustomPlot Thermal", QCPColorGradient::gpThermal},
                    {"↔QCustomPlot Polar", QCPColorGradient::gpPolar},
                    {"↔QCustomPlot Spectrum", QCPColorGradient::gpSpectrum},
                    {"QCustomPlot Jet", QCPColorGradient::gpJet},
                    {"QCustomPlot Hues", QCPColorGradient::gpHues},
                    {"QCustomPlot Hot", QCPColorGradient::gpHot},
                    {"QCustomPlot Cold", QCPColorGradient::gpCold},
                    {"↔ColorBrewer BrownBlueGreen", QCPColorGradient::cbBrBG},
                    {"↔ColorBrewer PinkYellowGreen", QCPColorGradient::cbPiYG},
                    {"↔ColorBrewer PurpleGreen", QCPColorGradient::cbPRGn},
                    {"↔ColorBrewer PurpleOrange", QCPColorGradient::cbPuOr},
                    {"↔ColorBrewer RedBlue", QCPColorGradient::cbRdBu},
                    {"↔ColorBrewer RedGray", QCPColorGradient::cbRdGy},
                    {"↔ColorBrewer RedYellowBlue", QCPColorGradient::cbRdYlBu},
                    {"↔ColorBrewer RedYellowGreen", QCPColorGradient::cbRdYlGn},
                    {"↔ColorBrewer Spectral", QCPColorGradient::cbSpectral},
                    {"↔Vespucci Spectral", QCPColorGradient::vSpectral}
                };
}

///
/// \brief VespucciWorkspace::~VespucciWorkspace
/// Destructor
VespucciWorkspace::~VespucciWorkspace()
{
    cout << "Workspace destructor\n";

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
void VespucciWorkspace::SetPointers(MainWindow *main_window, DatasetTreeModel *tree_model)
{
    main_window_ = main_window;
    dataset_tree_model_ = tree_model;
}


// FUNCTIONS THAT ADD AND REMOVE WORKSPACE OBJECTS

///
/// \brief VespucciWorkspace::AddDataset
/// \param dataset
/// Add a dataset to the datasets_ QList
void VespucciWorkspace::AddDataset(QSharedPointer<VespucciDataset> dataset)
{
    data_model_->AddDataset(dataset);
    main_window_->RefreshTreeModel(data_model_);
    ++dataset_loading_count_;
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
    main_window_->RefreshTreeModel(data_model_);
}

///
/// \brief VespucciWorkspace::UpdateModel
/// Call when a VespucciDataset adds or removes a new object.
void VespucciWorkspace::UpdateModel()
{
    main_window_->RefreshTreeModel(data_model_);
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
    TreeItem *item = main_window_->dataset_tree_model()->getItem(parent);
    QStringList keys = item->keys();
    if (keys.size() == 1) RemoveDataset(keys[0]);
    else return;
}

QSharedPointer<VespucciDataset> VespucciWorkspace::DatasetAt(const QModelIndex &parent)
{
    TreeItem *item = main_window_->dataset_tree_model()->getItem(parent);
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
    return QSharedPointer<VespucciDataset>(data_model_->GetDataset(key));
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

QSharedPointer<MapData> VespucciWorkspace::GetMap(const QStringList &map_keys)
{
    return data_model_->GetMap(map_keys);
}

const mat & VespucciWorkspace::GetAuxiliaryMatrix(const QString &dataset_key, const QString &matrix_key) const
{
    return data_model_->GetAuxiliaryMatrix(dataset_key, matrix_key);
}

const mat & VespucciWorkspace::GetResultsMatrix(const QString &dataset_key, const QString &results_key, const QString &matrix_key) const
{
    return data_model_->GetResultsMatrix(dataset_key, results_key, matrix_key);
}

const mat & VespucciWorkspace::GetCoreMatrix(const QString &dataset_key, const QString &matrix_key) const
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
/// \brief VespucciWorkspace::RemoveColorRange
/// \param name
/// Removes a color range from the workspace.
/// Maps using this color range are not affected.
/// User must manually trigger recalculation per map
void VespucciWorkspace::RemoveColorRange(QString name)
{
    if (global_gradients_.contains(name)) global_gradients_.remove(name);
}

QStringList VespucciWorkspace::GlobalGradientKeys()
{
    return global_gradients_.keys();
}

QMap<QString, Vespucci::GlobalGradient> VespucciWorkspace::global_gradients()
{
    return global_gradients_;
}

QCPColorGradient VespucciWorkspace::GetGradient(QString key, int count)
{
    if (key == "ColorBrewer Cluster"){
        switch (count){
            case 2: return QCPColorGradient::cbCluster2;
            case 3: return QCPColorGradient::cbCluster3;
            case 4: return QCPColorGradient::cbCluster4;
            case 5: return QCPColorGradient::cbCluster5;
            case 6: return QCPColorGradient::cbCluster6;
            case 7: return QCPColorGradient::cbCluster7;
            case 8: return QCPColorGradient::cbCluster8;
            case 9: default: return QCPColorGradient::cbCluster9;
        }
    }
    if (gradients_.contains(key))
        return gradients_.value(key);
    else
        return QCPColorGradient::gpJet;
}

Vespucci::GlobalGradient VespucciWorkspace::GetGlobalGradient(QString key)
{
    return global_gradients_.value(key);
}

QStringList VespucciWorkspace::GradientNames(bool include_cluster)
{
    QStringList keys = gradients_.keys();
    if (include_cluster)
        keys.insert(0, "ColorBrewer Cluster");
    return keys;
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
/// \brief VespucciWorkspace::AddGlobalGradient
/// \param name
/// \param gradient_key
/// \param lower
/// \param upper
///
void VespucciWorkspace::AddGlobalGradient(QString name, QString gradient_key, double lower, double upper)
{
    Vespucci::GlobalGradient gradient;
    gradient.gradient = GetGradient(gradient_key);
    gradient.range = QCPRange(lower, upper);
    global_gradients_[name] = gradient;
}

void VespucciWorkspace::RecalculateGlobalGradient(QString name)
{
    if (global_gradients_.contains(name)){
        QList<QSharedPointer<MapData> > maps =
                data_model_->GetMapsUsingColorRange(name);
        if (maps.isEmpty()) return;
        vec mins(maps.size());
        vec maxes(maps.size());
        for (uword i = 0; i < mins.n_elem; ++i){
            mins(i) = maps[i]->min();
            maxes(i) = maps[i]->max();
        }
        double new_min = mins.min();
        double new_max = maxes.max();

        Vespucci::GlobalGradient gradient = global_gradients_.value(name);
        gradient.range = QCPRange(new_min, new_max);

        for (auto map: maps){
            map->SetGlobalGradient(name);
        }
    }
}

///
/// \brief VespucciWorkspace::ClearDatasets
/// Clears the internal container for datasets in the list model
void VespucciWorkspace::ClearDatasets()
{
    main_window_->dataset_tree_model()->ClearDatasets();
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
    return main_window_->dataset_tree_model();
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

DataModel *VespucciWorkspace::data_model()
{
    return data_model_;
}

///
/// \brief VespucciWorkspace::GetMatrix
/// \param keys
/// \return
/// Retruns empty matrix if invalid keys
const mat & VespucciWorkspace::GetMatrix(const QStringList &keys) const
{
    if (keys.size() < 2) return data_model_->EmptyMatrix();
    try{
        return data_model_->GetMatrix(keys);
    }catch(exception e){
        main_window_->DisplayExceptionWarning("VespucciWorkspace::GetMatrix", e);
        return data_model_->EmptyMatrix();
    }
}

bool VespucciWorkspace::HasMatrix(const QStringList &keys) const
{
    return data_model_->HasMatrix(keys);
}

bool VespucciWorkspace::Mappable(const QStringList &keys) const
{
    return data_model_->Mappable(keys);
}

bool VespucciWorkspace::Plottable(const QStringList &keys) const
{
    try{
        return data_model_->Plottable(keys);
    }catch(exception e){
        main_window_->DisplayExceptionWarning("VespucciWorkspace::Plottable", e);
        return false;
    }
}


void VespucciWorkspace::UpdateTreeModel()
{
    main_window_->dataset_tree_model()->UpdateData(data_model_);
}
