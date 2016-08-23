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

    valid_commands_["MinMaxNormalize"] = QStringList();
    valid_commands_["MeanCenter"] = QStringList();
    valid_commands_["ZScoreNormalize"] = QStringList();
    valid_commands_["AbsoluteValue"] = QStringList();
    valid_commands_["ShedZeroSpectra"] = QStringList();
    valid_commands_["ShedZeroWavelengths"] = QStringList();
    valid_commands_["VectorNormalize"] = QStringList({ "UInt" });
    valid_commands_["SNVNormalize"] = QStringList({ "Double", "Bool" });
    valid_commands_["PeakIntensityNormalize"] = QStringList({ "Double", "Double" });
    valid_commands_["Booleanize"] = QStringList({ "Double" , "Double" , "Bool" , "Bool" });
    valid_commands_["Clamp"] = QStringList({ "Double" , "Double" });
    valid_commands_["MedianFilter"] = QStringList({ "UInt" });
    valid_commands_["LinearMovingAverage"] = QStringList({ "UInt" });
    valid_commands_["SavitzkyGolay"] = QStringList({ "UInt", "UInt", "UInt" });
    valid_commands_["SingularValue"] = QStringList({ "UInt" });
    valid_commands_["QUIC_SVD"] = QStringList({ "Double" });
    valid_commands_["MFBaseline"] = QStringList({ "Int", "Int" });
    valid_commands_["IModPolyBaseline"] = QStringList({ "UInt", "UInt", "Double" });
    valid_commands_["RemoveClippedSpectra"] = QStringList({ "Double" });
    valid_commands_["RemoveFlatSpectra"] = QStringList({ "Double" });
    valid_commands_["ZeroClippedSpectra"] = QStringList({ "Double" });
    valid_commands_["ZeroFlatSpectra"] = QStringList({ "Double" });
    valid_commands_["Scale"] = QStringList({ "Double" });
    valid_commands_["ShedSpectrum"] = QStringList({ "UInt" });
    valid_commands_["ZeroSpectrum"] = QStringList({ "UInt" });
    valid_commands_["Univariate"] = QStringList({ "String" , "Double" , "Double" , "UInt" });
    valid_commands_["BandRatio"] = QStringList({ "String" , "Double" , "Double" , "Double" , "Double" , "UInt" });
    valid_commands_["PartialLeastSquares"] = QStringList({ "String", "UInt" });
    valid_commands_["VertexComponents"] = QStringList({ "String", "UInt" });
    valid_commands_["KMeans"] = QStringList({ "String", "UInt" , "String" });
    valid_commands_["PrincipalComponents"] = QStringList({ "String" });
    valid_commands_["ClassicalLeastSquares"] = QStringList({"String", "String", "String", "String"});
}

///
/// \brief MacroParser::LoadMacro
/// \param macro
/// Parses string containing the macro.
/// Commands must be separated by newlines and cannot be nested.
/// If not on list of valid commands (or not formatted correctly), user will be
/// warned of syntax error if this returns false
bool MacroParser::LoadMacro(QString macro)
{

    QStringList command_list = macro.split("\n");
    //this regular expression is used to split the commands so that the first
    //member of the list is the function name, and all subsequent entries are
    //parameters of the function.
    QRegExp sep("(\\(|\\)|,|\\s)");

    bool command_valid;
    for (int i = 0; i < command_list.size(); ++i){
        QStringList pieces = command_list[i].split(sep, QString::SkipEmptyParts);
        if (!pieces.size()) continue;
        QString function_name = pieces.first();
        pieces.removeAt(0);
        command_valid = ValidateCommand(function_name, pieces, error_param_);
        if (!command_valid){
            error_line_ = i;
            return false;
        }
        else{
            QPair<QString, QStringList> processed_command(function_name, pieces);
            commands_.push_back(processed_command);
        }
    }

   return true;
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
    for(auto command: commands_){
        ExecuteCommand(command.first, command.second);
    }
    return true; //we've already handled the cases where it would return false
}

void MacroParser::Error(int &error_line, int &error_param)
{
    error_line = error_line_;
    error_param = error_param_;
}

///
/// \brief MacroParser::ExecuteCommand
/// \param command
/// This is a private member function that executes a single command. The list
/// of valid commands is parsed. Commands are validated by ValidateCommand before
/// this function is called by ExecuteMacro(). This command must make the appropriate
/// type conversion (QString, int, uint, double) of the parameters.
void MacroParser::ExecuteCommand(QString command, QStringList params)
{
	if (command == "MinMaxNormalize")
        dataset_->MinMaxNormalize();
	else if (command == "VectorNormalize")
        dataset_->VectorNormalize(params[0].toInt());
	else if (command == "MeanCenter")
        dataset_->MeanCenter();
	else if (command == "ZScoreNormalize")
        dataset_->ZScoreNormalize();
	else if (command == "AbsoluteValue")
        dataset_->AbsoluteValue();
	else if (command == "ShedZeroSpectra")
        dataset_->ShedZeroSpectra();
	else if (command == "ShedZeroWavelengths")
        dataset_->ShedZeroWavelengths();
	else if (command == "SNVNormalize")
        dataset_->SNVNormalize(params[0].toDouble(), ToBool(params[1]));
	else if (command == "PeakIntensityNormalize")
        dataset_->PeakIntensityNormalize(params[0].toDouble(), params[1].toDouble());
	else if (command == "Booleanize")
        dataset_->Booleanize(params[0].toDouble(), params[1].toDouble(), ToBool(params[2]), ToBool(params[3]));
	else if (command == "Clamp")
        dataset_->Clamp(params[0].toDouble(), params[1].toDouble());
	else if (command == "MedianFilter")
        dataset_->MedianFilter(params[0].toInt());
	else if (command == "LinearMovingAverage")
        dataset_->LinearMovingAverage(params[0].toInt());
	else if (command == "SavitzkyGolay")
        dataset_->SavitzkyGolay(params[0].toInt(), params[1].toInt(), params[2].toInt());
	else if (command == "SingularValue")
        dataset_->SingularValue(params[0].toInt());
	else if (command == "QUIC_SVD")
        dataset_->QUIC_SVD(params[0].toDouble());
	else if (command == "MFBaseline")
        dataset_->MFBaseline(params[0].toInt(), params[1].toInt());
	else if (command == "IModPolyBaseline")
        dataset_->IModPolyBaseline(params[0].toInt(), params[1].toInt(), params[2].toDouble());
	else if (command == "RemoveClippedSpectra")
        dataset_->RemoveClippedSpectra(params[0].toDouble());
	else if (command == "RemoveFlatSpectra")
        dataset_->RemoveFlatSpectra(params[0].toDouble());
    else if (command == "ZeroFlatSpectra")
        dataset_->ZeroFlatSpectra(params[0].toDouble());
    else if (command == "ZeroClippedSpectra")
        dataset_->ZeroClippedSpectra(params[0].toDouble());
	else if (command == "Scale")
        dataset_->Scale(params[0].toDouble());
	else if (command == "ShedSpectrum")
        dataset_->ShedSpectrum(params[0].toInt());
    else if (command == "ZeroSpectrum")
        dataset_->ZeroSpectrum(params[0].toInt());
    else if (command == "Univariate"){
        double param1 = params[1].toDouble();
        double param2 = params[2].toDouble();
        dataset_->Univariate(params[0], param1, param2, params[3].toInt());
    }
    else if (command == "BandRatio"){
        double param1 = params[1].toDouble();
        double param2 = params[2].toDouble();
        double param3 = params[3].toDouble();
        double param4 = params[4].toDouble();
        dataset_->BandRatio(params[0], param1, param2, param3, param4, params[5].toInt());
    }
	else if (command == "PartialLeastSquares")
        dataset_->PartialLeastSquares(params[0], params[1].toInt());
	else if (command == "VertexComponents")
        dataset_->VertexComponents(params[0], params[1].toInt());
	else if (command == "KMeans")
        dataset_->KMeans(params[0], params[1].toInt(), params[2]);
	else if (command == "PrincipalComponents")
        dataset_->PrincipalComponents(params[0]);
    else if (command == "ClassicalLeastSquares"){
        QString name = params[0];
        QStringList keys = params;
        params.removeFirst();
        dataset_->ClassicalLeastSquares(name, keys);
    }
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
    bool conversion_ok;
	
	if (expected_types.size() != params.size()) {
		param_error = 0;
		return false;
	}
	for (int i = 0; i < params.size(); ++i) {
        if (expected_types[i] == "Bool")
			conversion_ok = (params[i].toLower() == "true" || params[i].toLower() == "false");
        else if (expected_types[i] == "Int")
			params[i].toInt(&conversion_ok);
        else if (expected_types[i] == "UInt") {
			int val = params[i].toInt(&conversion_ok);
			conversion_ok = (conversion_ok && val >= 0);
		}
        else if (expected_types[i] == "Double")
			params[i].toDouble(&conversion_ok);
		else conversion_ok = true; //Treating as String

		if (!conversion_ok) {
            param_error = i + 1;
			return false;
		}
	}
    return true;
}

bool MacroParser::ToBool(const QString &param)
{
    return (param.toLower() == "true");
}
