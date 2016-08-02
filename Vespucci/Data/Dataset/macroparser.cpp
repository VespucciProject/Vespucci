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
//Skeleton functions to be filled by GABowers

#include "macroparser.h"

MacroParser::MacroParser(QSharedPointer<VespucciDataset> dataset)
{
    dataset_ = dataset;
}

///
/// \brief MacroParser::LoadMacro
/// \param macro
/// Parses string containing the macro.
/// Commands must be separated by newlines and cannot be nested.
/// If not on list of valid commands (or not formatted correctly), User will be
/// warned of syntax err of this returns false
bool MacroParser::LoadMacro(QString macro)
{
    //if the function takes no parameters, use an empty string list
    //Initializer lists for Qt containers are not supported by Visual Studio yet,
    //so we must create an empty current_params object;
    QStringList current_params;
    //Do the commands that take no arguments first
    valid_commands_["MinMaxNormalize"] = current_params;

    //After adding a command that takes an argument, clear current_params;
    current_params << "Double" << "Int";
    valid_commands_["SNVNormalize"] = current_params;
    current_params.clear();



    QStringList command_list = macro.split("\n");
    //this regular expression is used to split the commands so that the first
    //member of the list is the function name, and all subsequent entries are
    //parameters of the function.
    QRegExp sep("(\(|\)|,)");
    for (auto command: command_list){
        QStringList pieces = command.split(sep);
        QString function_name = pieces[0];
        pieces.removeAt(0); //now pieces has only the parameters
        QPair<QString, QStringList> processed_command(function_name, pieces);
        commands_.push_back(processed_command);//insert at the end of the list
    }

    return false;
}

///
/// \brief MacroParser::ExecuteMacro
/// Iteratively executes the commands in commands_. Each command is first validated.
/// If any command is invalid, none execute. The method returns false whenever
/// any invalid command is found. The caller only uses error_line and error_param
/// if this function returns false.
bool MacroParser::ExecuteMacro()
{
    bool ok;
    for (int i = 0; i < commands_.size(); ++i){
        ok = ValidateCommand(commands_[i].first, commands_[i].second, error_param_);
        if (!ok){
            error_line_ = i + 1; //indexing starts at 1 here because to the user there is no line 0.
            error_param_++; //indexing starts at 1 here because we count the name of the function as parameter 0.
            return false;
        }
    }

    //if all commands are valid, execute each in order
    for(int i = 0; i < commands_.size(); ++i){
        ExecuteCommand(commands_[i].first, commands_[i].second);
    }

    return true; //we've already handled the cases where it would return false
}

///
/// \brief MacroParser::ExecuteCommand
/// \param command
/// This is a private member function that executes a single command. The list
/// of valid commands is parsed. Commands are validated by ValidateCommand before
/// this function is called by ExecuteMacro(). This command must make the appropriate
/// type conversion (QString, int, unint, double) of the parameters.
void MacroParser::ExecuteCommand(QString command, QStringList params)
{

}

///
/// \brief MacroParser::ValidateCommand
/// \param command The string containing the command
/// \param params The string list containing entered arguments
/// \param param_error The parameter where the error occurred. This should only
/// be checked by the caller when
/// \return true if command is valid, false if not.
/// This function validates a command by making sure its definition is in the valid list
/// and checks to make sure that the proper number and type is used. If the parameter
/// is surrounded in quotes, it is interpreted as a string. If the parameter has a
/// decimal point, it is interpreted as a double. If the parameter has no quotes or
/// decimal places, it is interpreted as uint if it has no negative sign and as
/// int if it has a negative sign. Functions that take type Int can handle UInt
/// parameters, but functions that take UInt cannot take Int types. If a parameter
/// consists entirely of the word "true" or "false" it is interpreted as a bool
/// with the approprate value. These parameters should not be surrounded by quotes
/// or they will instead be interpreted as strings.
///
bool MacroParser::ValidateCommand(QString command, QStringList params, int &param_error)
{
    //the QString class has members named toDouble and toInt that take pointers
    //to bools and change the values based on success or failure to convert
    bool conversion_ok;

    //iterate through commands (first member of pair), checking that it exists
    //as a key in the valid map, and getting it's parameter type list.

    //iterate through parameter list for this command, keeping count of what parameter we're on

    //if conversion fails, set param_error to the parameter we're on (+1 if you start counting at 0) and return false




    //The input_type will be "Bool, Int, UInt, String, Double, Invalid"
    QString input_type;
    return false;
}
