/*******************************************************************************
    Copyright (C) 2015 Wright State University - All Rights Reserved
    Daniel P. Foose - Author

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
#include "GUI/mainwindow.h"
#include <QApplication>
#include "Data/Dataset/vespuccidataset.h"
#include <QTextStream>
#include <QFileDevice>
#include <QFile>
#include <qcustomplot.h>
#include <fstream>
#include "Data/Imaging/mapdata.h"
#include "Global/vespucciworkspace.h"
#include <boost/program_options.hpp>
#include <cassert>

///
/// \brief main
/// \param argc
/// \param argv
/// \return
/// Typical boilerplate C++ main() stuff. Instantiates workspace and main window.
int main(int argc, char *argv[])
{

    //Launch QApplication instance
    QCoreApplication::setOrganizationName("Wright State University");
    QCoreApplication::setOrganizationDomain("chm.wright.edu");
    QCoreApplication::setApplicationName("Vespucci");
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
    QApplication a(argc, argv);
    //A pointer to this goes by "workspace" in every window that needs it
    VespucciWorkspace ws;

    //Clean up dataset log files from when it crashed last
    ws.CleanLogFiles();


#ifdef QT_NO_DEBUG
    std::freopen("stdout_log.txt", "w", stdout);
    std::freopen("stderr_log.txt", "w", stderr);
#endif

    //Instantiate main window
    MainWindow w(0, &ws);

    //This "finishes construction" on ws, for the parts that come from w
    ws.SetPointers(&w);

    //show main window
    w.show();
    return a.exec();
    try{
        a.exec();
    }catch(std::exception e){
        //save session
        //rename stderr_log and stdout_log

    }
}


