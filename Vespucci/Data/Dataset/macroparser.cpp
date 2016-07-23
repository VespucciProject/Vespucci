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
	valid_commands_["VectorNormalize"] = current_params;
	valid_commands_["MeanCenter"] = current_params;
	valid_commands_["ZScoreNormalize"] = current_params;
	valid_commands_["AbsoluteValue"] = current_params;
	valid_commands_["UnitAreaNormalize"] = current_params;
	valid_commands_["ShedZeroSpectra"] = current_params;
	valid_commands_["ShedZeroWavelengths"] = current_params;

	valid_commands_["SNVNormalize"] = { "Double", "Bool" };

	valid_commands_["PeakIntensityNormalize"] = { "Double", "Double" };

	valid_commands_["Booleanize"] = { "Double" , "Double" , "Bool" , "Bool" };

	valid_commands_["Clamp"] = { "Double" , "Double" };

	valid_commands_["MedianFilter"] = { "UInt" };

	valid_commands_["LinearMovingAverage"] = { "UInt" };

	valid_commands_["SavitzkyGolay"] = { "UInt", "UInt", "UInt" };

	valid_commands_["SingularValue"] = { "UInt" };

	valid_commands_["QUIC_SVD"] = { "Double" };

	valid_commands_["MFBaseline"] = { "Int", "Int" };

	valid_commands_["IModPolyBaseline"] = { "UInt", "UInt", "Double" };

	valid_commands_["RemoveClippedSpectra"] = { "Double" };

	valid_commands_["RemoveFlatSpectra"] = { "Double" };

	valid_commands_["Scale"] = { "Double" };

	valid_commands_["ShedSpectrum"] = { "UInt" };

	//These are ANALYSIS commands
	valid_commands_["Univariate"] = { "String" , "Double" , "Double" , "UInt" };

	valid_commands_["BandRatio"] = { "String" , "Double" , "Double" , "Double" , "Double" , "UInt" };

	valid_commands_["PartialLeastSquares"] = { "String", "UInt" };

	valid_commands_["VertexComponents"] = { "String", "UInt" };

	valid_commands_["KMeans"] = { "String", "String", "UInt" };

	valid_commands_["PrincipalComponents"] = { "String" };

    QStringList command_list = macro.split("\n");
    //this regular expression is used to split the commands so that the first
    //member of the list is the function name, and all subsequent entries are
    //parameters of the function.
    QRegExp sep("(\(|\)|,)");
    foreach (const QString &command, command_list){
        QStringList pieces = command.split(sep);
        QString function_name = pieces[0];
        pieces.removeAt(0); //now pieces has only the parameters
        QMap<QString, QStringList> processed_command(function_name, pieces);
        commands_.push_back(processed_command);//insert at the end of the list
    }
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
    foreach(QPair<QString, QStringList> command, commands_){
        ExecuteCommand(command.first, command.second);
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
	if (command == "MinMaxNormalize")
		dataset->MinMaxNormalize();
	else if (command == "VectorNormalize")
		dataset->VectorNormalize();
	else if (command == "MeanCenter")
		dataset->MeanCenter();
	else if (command == "ZScoreNormalize")
		dataset->ZScoreNormalize();
	else if (command == "AbsoluteValue")
		dataset->AbsoluteValue();
	else if (command == "UnitAreaNormalize")
		dataset->UnitAreaNormalize();
	else if (command == "ShedZeroSpectra")
		dataset->ShedZeroSpectra();
	else if (command == "ShedZeroWavelengths")
		dataset->ShedZeroWavelengths();
	else if (command == "SNVNormalize")
		dataset->SNVNormalize(params[0].toDouble(), ToBool(params[1]));
	else if (command == "PeakIntensityNormalize")
		dataset->PeakIntensityNormalize(params[0].toDouble(), params[1].toDouble());
	else if (command == "Booleanize")
		dataset->Booleanize(params[0].toDouble(), params[1].toDouble(), ToBool(params[2]), ToBool(params[3]));
	else if (command == "Clamp")
		dataset->Clamp(params[0].toDouble(), params[1].toDouble());
	else if (command == "MedianFilter")
		dataset->MedianFilter(params[0].toInt());
	else if (command == "LinearMovingAverage")
		dataset->LinearMovingAverage(params[0].toInt());
	else if (command == "SavitzkyGolay")
		dataset->SavitzkyGolay(params[0].toInt(), params[1].toInt(), params[2].toInt());
	else if (command == "SingularValue")
		dataset->SingularValue(params[0].toInt());
	else if (command == "QUIC_SVD")
		dataset->QUIC_SVD(params[0].toDouble());
	else if (command == "MFBaseline")
		dataset->MFBaseline(params[0].toInt(), params[1].toInt());
	else if (command == "IModPolyBaseline")
		dataset->IModPolyBaseline(params[0].toInt(), params[1].toInt(), params[2].toDouble());
	else if (command == "RemoveClippedSpectra")
		dataset->RemoveClippedSpectra(params[0].toDouble());
	else if (command == "RemoveFlatSpectra")
		dataset->RemoveFlatSpectra(params[0].toDouble());
	else if (command == "Scale")
		dataset->Scale(params[0].toDouble());
	else if (command == "ShedSpectrum")
		dataset->ShedSpectrum(params[0].toInt());
	else if (command == "Univariate")
		dataset->Univariate(params[0], params[1].toDouble(), params[2].toDouble(), params[3].toInt());
	else if (command == "BandRatio")
		dataset->BandRatio(params[0], params[1].toDouble(), params[2].toDouble(), params[3].toDouble(), params[4].toDouble(), params[5].toInt());
	else if (command == "PartialLeastSquares")
		dataset->PartialLeastSquares(params[0], params[1].toInt());
	else if (command == "VertexComponents")
		dataset->VertexComponents(params[0], params[1].toInt());
	else if (command == "KMeans")
		dataset->KMeans(params[0], params[1], params[2].toInt());
	else if (command == "PrincipalComponents")
		dataset->PrincipalComponents(params[0]);
	else; //Do nothing
}

bool ToBool(const QString &param) {
	return param.toLower() == "true";
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
	QStringList expected_types = valid_commands_[command];
	
	if (expected_types.size() != params.size()) {
		param_error = 0;
		return false;
	}
	for (int i = 0; i < params.size(); ++i) {
		if (expected_type == "Bool")
			conversion_ok = (params[i].toLower() == "true" || params[i].toLower() == "false");
		else if (expected_type == "Int")
			params[i].toInt(&conversion_ok);
		else if (expected_type == "UInt") {
			int val = params[i].toInt(&conversion_ok);
			conversion_ok = (conversion_ok && val >= 0);
		}
		else if (expected_type == "Double")
			params[i].toDouble(&conversion_ok);
		else conversion_ok = true; //Treating as String

		if (!conversion_ok) {
			param_error = i + 1;
			return false;
		}
	}
	return true;
}
