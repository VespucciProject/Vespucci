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

#include "specmap.h" //specmap includes all necessary headers.
#include <cmath>

using namespace arma;
using namespace std;

SpecMap::SpecMap()
{

}

SpecMap::SpecMap(QTextStream &inputstream, QMainWindow *main_window)
{
    //Set up variables unrelated to hyperspectral data:
    map_list_widget_ = main_window->findChild<QListWidget *>("mapsListWidget");
    map_loading_count_ = 0;



    int i, j;
    wall_clock timer;

    /*Read the first line to get the wavelength*/
    inputstream.seek(0);
    cout << "Loading wavelength vector..." << endl;
    timer.tic();
    QString wavelength_string = inputstream.readLine();

    QStringList wavelength_string_list =
            wavelength_string.split("\t",  QString::SkipEmptyParts);

    int columns = wavelength_string_list.size();
    wavelength_.set_size(columns);

    for(i=0; i<columns; ++i){
        wavelength_(i) = wavelength_string_list.at(i).toDouble();
    }
    double seconds = timer.toc();
    cout << "Reading wavelength took " << seconds <<" s." << endl;
    //cout << "Wavelength vector:";
    //wavelength_.print(cout);
    i=0;
    j=0;


    QString spectra_string;

    QStringList spectra_string_list;
    QProgressDialog progress("Counting rows...", "Cancel", 0, 100, NULL);
    progress.setWindowTitle("Loading Dataset");
    progress.setWindowModality(Qt::WindowModal);

    int rows = 0;
    cout << "Counting rows..." << endl;
    timer.tic();
    while(inputstream.readLine()!=NULL){
        ++rows;
    }
    progress.setValue(1);
    progress.setRange(0,rows+1);

    spectra_.set_size(rows, columns);
    x_.set_size(rows);
    y_.set_size(rows);
    seconds = timer.toc();
    cout << "Counting rows and resizing took " << seconds << " s." << endl;
    cout << "Reading spectra, x, and y..." << endl;

    progress.setLabelText("Parsing spectra...");

    timer.tic();
    inputstream.seek(0);
    inputstream.readLine(); //discard it to advance to next line

    for(i=0; i<rows; ++i){
        spectra_string=inputstream.readLine();
        spectra_string_list =
                spectra_string.split("\t", QString::SkipEmptyParts);

        x_(i) = spectra_string_list.at(0).toDouble();
        spectra_string_list.removeAt(0);

        y_(i) = spectra_string_list.at(0).toDouble();
        spectra_string_list.removeAt(0);
        for (j=0; j<columns; ++j){
            spectra_(i,j) = spectra_string_list.at(j).toDouble();
        }
        progress.setValue(i);
    }
    seconds = timer.toc();
    cout << "Reading x, y, and spectra took " << seconds << " s." << endl;

}


// MAPPING FUNCTIONS //


void SpecMap::Univariate(int min,
                         int max,
                         QString name,
                         QString value_method,
                         int gradient_index,
                         bool interpolation)
{

    cout << "SpecMap::Univariate" << endl;

    unsigned int size = x_.n_elem;
    unsigned int i;

    const QCPRange value_range = this->ValueRange();
    const QCPRange key_range = this->KeyRange();
    int key_size = this->KeySize();
    int value_size = this->ValueSize();

    MapData* map = new MapData(x_axis_description_, y_axis_description_, this);

    map->set_name(name);
    map->set_type("1-Region Univariate");

    QCPColorMapData map_data(key_size, value_size, key_range, value_range);

    rowvec region;
    colvec results;
    results.set_size(size);

    cout << "conditionals based on peak determination method" << endl;

    if(value_method == "area"){
        // Do peak fitting stuff here.
    }

    else if(value_method == "derivative"){
        // Do derivative stuff here
    }

    else{
        // Makes an intensity map
        cout << "conditional for intensity map" << endl;
        map->set_type("1-Region Univariate (Intensity)");
        cout << "line 157" <<endl;
        for (i=0; i<size; ++i){
            region = spectra_(i, span(min, max));
            results(i)=region.max();
        }
    }

    //double results_min = results.min();
    //load data
    for (i=0; i<size; i++){
        //results(i) = results(i) - results_min;
        map_data.setData(x_(i), y_(i), results(i));
    }

    map->SetMapData(&map_data);
    this->AddMap(map);
    MapData *map_ptr = maps_.last();
    map_ptr->CreateImage(this->GetGradient(gradient_index), interpolation);
    map_ptr->ShowMapWindow();
}

void SpecMap::BandRatio(int first_min,
                        int first_max,
                        int second_min,
                        int second_max,
                        QString name,
                        QString value_method)
{

    cout << "SpecMap::BandRatio" << endl;

    unsigned int size = x_.n_elem;
    unsigned int i;

    const QCPRange value_range = this->ValueRange();
    const QCPRange key_range = this->KeyRange();
    int key_size = this->KeySize();
    int value_size = this->ValueSize();

    MapData* map = new MapData(x_axis_description_, y_axis_description_, this);

    map->set_name(name);
    map->set_type("1-Region Univariate");

    QCPColorMapData map_data(key_size, value_size, key_range, value_range);

    rowvec first_region;
    rowvec second_region;
    colvec results;
    results.set_size(size);

    cout << "conditionals based on peak determination method" << endl;

    if(value_method == "area"){
        // Do peak fitting stuff here.
    }

    else if(value_method == "derivative"){
        // Do derivative stuff here
    }

    else{
        // Makes an intensity map
        cout << "conditional for intensity map" << endl;
        map->set_type("1-Region Univariate (Intensity)");
        cout << "line 157" <<endl;
        for (i=0; i<size; ++i){
            first_region = spectra_(i, span(first_min, first_max));
            second_region = spectra_(i, span(second_min, second_max));
            results(i)= first_region.max()/second_region.max();
        }
    }

    //double results_min = results.min();
    //load data
    for (i=0; i<size; i++){
        //results(i) = results(i) - results_min;
        map_data.setData(x_(i), y_(i), results(i));
    }

    map->SetMapData(&map_data);
    this->AddMap(map);
    MapData *map_ptr = maps_.last();
    map_ptr->CreateImage(QCPColorGradient::gpHot, false); //values will be determined later
    map_ptr->ShowMapWindow();
}


vector<int> SpecMap::FindRange(double start, double end)
{
    cout << "SpecMap::FindRange" << endl;
    vector<int> indices(2,0);
    int length = wavelength_.size();
    int i;
    cout << "looping through wavelength_ for start" << endl;
    for (i=0; i<length; ++i){
        if(wavelength_(i)>=start){
            break;
        }
    }
    if (i==length-1){
        cerr << "Can't find index of start of range!" << endl;
        return indices; //in this case, indices contains only zeroes
    }

    if (fabs(wavelength_(i)-start)>fabs(wavelength_(i-1)-start)){
        indices[0]=i-1;
    }

    else{
        indices[0]= i;
    }

    cout <<"looping through wavelength until end found" <<endl;
    for (i=i; i<length; ++i){
        if(wavelength_(i)>=end){
            break;
        }
    }

    if(i==length-1){
        cerr << "Can't find index of upper wavelength limit!" << endl;
        cerr << "Setting upper limit equal to lower limit (point ROI)" << endl;
        indices[1]=indices[0];
        return indices;
    }

    if (fabs(wavelength_(i)-start)>fabs(wavelength_(i-1)-start)){
        indices[1] = i-1;
    }

    else{
        indices[1] = i;
    }

    return indices;
}

// HELPER FUNCTIONS (Will go in own file later to speed compilation //
QVector<double> SpecMap::PointSpectrum(int index)
{
    std::vector<double> spectrum_stdvector =
            conv_to< std::vector<double> >::from(spectra_.row(index));

    QVector<double> spectrum_qvector =
            QVector<double>::fromStdVector(spectrum_stdvector);

    return spectrum_qvector;
}

QVector<double> SpecMap::WavelengthQVector()
{
    std::vector<double> wavelength_stdvector =
            conv_to< std::vector<double> >::from(wavelength_);

    QVector<double> wavelength_qvector =
            QVector<double>::fromStdVector(wavelength_stdvector);

    return wavelength_qvector;
}


QCPRange SpecMap::ValueRange()
{
    cout << "SpecMap::ValueRange" << endl;
    double lower = y_.min();
    double upper = y_.max();
    QCPRange range(upper, lower);
    return range;
}

QCPRange SpecMap::KeyRange()
{
    cout << "SpecMap::KeyRange" << endl;
    double lower = x_.min();
    double upper = x_.max();
    QCPRange range(upper, lower);
    return range;
}

int SpecMap::KeySize()
{
    cout << "SpecMap::KeySize" << endl;

    unsigned int i;
    int x_count=1;  //this counts the first entry in x_
    double x_buf = x_(0);

    //loop through x until a value different then the first is met.
    for(i=0; i<x_.n_elem; ++i){
        if(x_(i)!=x_buf){
            ++x_count;
            x_buf=x_(i);
        }
    }

    cout << "return value: " << x_count << endl;
    return x_count;
}

int SpecMap::ValueSize()
{
    cout << "SpecMap::ValueSize" << endl;

    unsigned int i = 0;
    int y_count = 0;


    //long-text files hold x constant and vary y
    //until x is different, count y

    for (i=0; i<x_.n_elem; ++i){
        if(x_(i)!=x_(0)){
            break;
        }
        else{
            ++y_count;
        }
    }


    cout << "return value:" << y_count << endl;
    return y_count;
}


// MEMBER ACCESS FUNCTIONS
rowvec SpecMap::wavelength()
{
    return wavelength_;
}

colvec SpecMap::x()
{
    return x_;
}

colvec SpecMap::y()
{
    return y_;
}

mat SpecMap::spectra()
{
    return spectra_;
}

const QString SpecMap::name()
{
    return name_;
}

void SpecMap::SetName(QString new_name)
{
    name_ = new_name;
}

//MAP HANDLING FUNCTIONS
QStringList SpecMap::map_names()
{
    return map_names_;
}

int SpecMap::map_loading_count()
{
    return map_loading_count_;
}

void SpecMap::RemoveMapAt(int i)
{
    QString name = map_names_.at(i);
    QListWidgetItem *item = map_list_widget_->takeItem(i);
    maps_.removeAt(i);
    map_names_.removeAt(i);
    map_list_widget_->removeItemWidget(item);
}

// Useful when only the name is easily known, and index won't be used
void SpecMap::RemoveMap(QString name)
{
    int i;
    for(i=0; i<map_names_.size(); ++i){
        if(map_names_.at(i)==name){
            this->RemoveMapAt(i);
        }
    }
}


void SpecMap::AddMap(MapData* map)
{
    cout << "SpecMap::AddMap" << endl;
    QString name = map->name();
    cout << "name: " << name.toStdString() << endl;
    cout << "adding map to list" << endl;
    maps_.append(map);
    cout << "adding map name to list" << endl;
    map_names_.append(name);
    cout << "adding map name to QListWidget" << endl;;

    map_list_widget_->addItem(name);
    cout<<"incrementing map count" << endl;
    ++map_loading_count_;
}

QCPRange SpecMap::WavelengthRange()
{
    double min = wavelength_.min();
    double max = wavelength_.max();
    QCPRange range(min, max);
    return range;
}

QCPRange SpecMap::PointSpectrumRange(int i)
{
    rowvec row = spectra_.row(i);
    double min = row.min();
    double max = row.max();

    QCPRange range(min, max);
    return range;
}

QCPColorGradient SpecMap::GetGradient(int gradient_number)
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
    case 38: return QCPColorGradient::cbCluster;
    default: return QCPColorGradient::gpCold;
    }
}
