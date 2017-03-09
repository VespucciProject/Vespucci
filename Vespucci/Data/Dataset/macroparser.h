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
#ifndef MACROPARSER_H
#define MACROPARSER_H

#include "Data/Dataset/vespuccidataset.h"
class MacroParser
{
public:
    MacroParser(QSharedPointer<VespucciDataset> dataset);
    bool LoadMacro(QString macro);
    bool ExecuteMacro();
    void Error(int &error_line, int &error_param);
private:
    void ExecuteCommand(QString command, QStringList params);
    bool ValidateCommand(QString command, QStringList params, int &param_error);
	bool ToBool(const QString &param);

    ///
    /// \brief commands_
    /// An container with the commands sorted in order of execution
    /// Each entry contains a map with the command head as the key and the command parameters as the value
    QList<QPair<QString, QStringList> > commands_;

    ///
    /// \brief valid_commands_
    /// The list of valid commands and the types of their parameters
    QMap<QString, QStringList> valid_commands_;

    ///
    /// \brief dataset_
    /// Dataset commands are performed on
    QSharedPointer<VespucciDataset> dataset_;

    ///
    /// \brief error_line_
    /// The line where the error occurred (indexed from 0)
    int error_line_;

    ///
    /// \brief error_param_
    /// The invalid parameter. If error_param_==0, then "Invalid Command", If
    /// any value greater than zero, "Invalid Argument", on the parameter described here
    int error_param_;
};

#endif // MACROPARSER_H
