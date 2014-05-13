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

#include "mainwindow.h"
#include <QApplication>
#include <armadillo>
#include "specmap.h"
#include <QTextStream>
#include <QFileDevice>
#include <QFile>
#include "qcustomplot.h"
#include <fstream>
#include "mapdata.h"
#include "vespucciworkspace.h"

using namespace std;
using namespace arma;

void DeleteDataset(QList<SpecMap> *datasets, QListWidget *dataset_list_widget, QString name);
void DeleteMap(QList<MapData> *maps, QListWidget *map_list_widget, QString name);

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    //A pointer to this goes by "workspace" in every window that needs it
    VespucciWorkspace ws;
    MainWindow w(0, &ws);
    //This "finishes construction" on ws, for the parts that come from w
    ws.SetPointers(&w);
    w.show();
    return a.exec();
}


