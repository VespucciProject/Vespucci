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

#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <QMainWindow>
#include "mapdata.h"

class MapData;

namespace Ui {
class MapViewer;
}

class MapViewer : public QMainWindow
{
    Q_OBJECT

public:
    MapViewer(MapData *data);
    //explicit MapViewer(MapData *data);
    ~MapViewer();

private slots:
    void on_actionInterpolate_triggered();

    void on_actionInterpolate_toggled(bool arg1);

private:
    Ui::MapViewer *ui;
    MapData *parent_;
};

#endif // MAPVIEWER_H
