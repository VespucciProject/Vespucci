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

///
/// \brief main
/// \param argc
/// \param argv
/// \return
/// Typical boilerplate C++ main() stuff. Instantiates workspace and main window.
int main(int argc, char *argv[])
{
    //logs of program output which can be viewed after program run.
    //This can be used for bug reports
    //Users can include stdout_log, stderr_log and the dataset log in bug reports
    //this is disabled for the debug config because I prefer to look at stderr and
    //stdout output directly when debugging.

#ifdef QT_NO_DEBUG
    std::freopen("stdout_log.txt", "w", stdout);
    std::freopen("stderr_log.txt", "w", stderr);
#endif

    //Launch QApplication instance
    QCoreApplication::setOrganizationName("Vespucci Project @ Wright State University");
    QCoreApplication::setOrganizationDomain("vespucciproject.org");
    QCoreApplication::setApplicationName("Vespucci");
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
    QApplication a(argc, argv);

    QSharedPointer<VespucciWorkspace> ws(new VespucciWorkspace(a.applicationDirPath()));

    //Instantiate main window
    MainWindow w(0, ws);

    //show main window
    w.show();
    return a.exec();
}


