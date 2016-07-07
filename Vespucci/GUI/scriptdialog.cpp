#include "GUI/scriptdialog.h"
#include "ui_scriptdialog.h"
#include <cstdlib>
#include <cstdio>
#include "External/fileinterprocess.h"

ScriptDialog::ScriptDialog(QWidget *parent, QSharedPointer<VespucciWorkspace> ws, const QString &dataset_key) :
    QDialog(parent),
    ui(new Ui::ScriptDialog)
{
    ui->setupUi(this);
    dataset_ = ws->GetDataset(dataset_key);
    workspace_ = ws;
}

ScriptDialog::~ScriptDialog()
{
    delete ui;
}

void ScriptDialog::on_buttonBox_accepted()
{
    //Parse input
    /*
    try{
        std::string cmd = ui->codePlainTextEdit->document()->toPlainText().toStdString();
        QStringList invars = ui->sendPlainTextEdit->document()->toPlainText().split("\n");
        QStringList outvars = ui->receivePlainTextEdit->document()->toPlainText().split("\n");

        //parse syntax for input box
        std::map<std::string, arma::mat> in_data;
        arma::mat *matrix_ptr;
        std::cout << "parsing invars..." << std::endl;
        for (int i = 0; i < invars.size(); ++i){
            std::cout << "invars[i] = " << invars[i].toStdString() << std::endl;
            if (invars[i] == "spectra")
                matrix_ptr = dataset_->spectra_ptr();
            else if (invars[i] == "abscissa")\
                matrix_ptr = dataset_->wavelength_ptr();
            else if (invars[i] == "x")
                matrix_ptr = dataset_->x_ptr();
            else if (invars[i] == "y")
                matrix_ptr = dataset_->y_ptr();
            else{
                matrix_ptr = dataset_->AnalysisResult(invars[i]);
            }

            if (matrix_ptr != NULL)
                in_data[invars[i].toStdString()] = *matrix_ptr;
        }

        std::cout << "parsing outvars" << endl;
        std::string interpreter_key;
        std::string vespucci_key;
        std::map<std::string, std::string> variable_keys;
        for (int i = 0; i < outvars.size(); ++i){
            std::cout << "outvars[i] = " << outvars[i].toStdString() << endl;
            vespucci_key = outvars[i].split("=")[0].trimmed().toStdString();
            interpreter_key = outvars[i].split("=")[1].trimmed().toStdString();
            std::cout << vespucci_key << ", " << interpreter_key << std::endl;
            variable_keys[vespucci_key] = interpreter_key;
        }
        std::map<std::string, arma::mat> data;
        std::cout << "R stuff" << endl;
        if (ui->interpreterComboBox->currentText() == "R"){
            workspace_->settings()->beginGroup("environment");
            QString r_home = workspace_->settings()->value("R_HOME").toString();
            workspace_->settings()->endGroup();
            r_home = "R_HOME=" + r_home;
            char* R_HOME;
            snprintf(R_HOME, r_home.size(), r_home.toStdString().c_str());
            putenv(R_HOME); //set R_HOME environment variable
            int argc = 1;
            char first_arg[] = "vespucci"; //done to avoid string conversion
            char* argv[1];
            argv[0] = first_arg;
            QScopedPointer<VespucciR> R_instance(new VespucciR(argc, argv));
            //send variables to R
            R_instance->SetEnvironment(in_data);
            //Run R code
            std::cout << "Run Script" << std::endl;
            R_instance->RunScript(cmd);
            //retrieve output from R
            std::cout << "Get Environment" << std::endl;
            data = R_instance->GetEnvironment(variable_keys);
            std::cout << "Add Results" << std::endl;
            dataset_->AddAnalysisResults(data);
        }
        else if (ui->interpreterComboBox->currentText() == "Octave"){
            //Do Octave stuff
        }
        else{
            QMessageBox::information(this, "Unknown or Unsupported Interpreter", "This interpreter is not usable from Vespucci", QMessageBox::Close);
        }
    }
    catch(exception e){
        cerr << e.what();
        QMessageBox::warning(this, "Exception Occurred", "An exception " + QString::fromStdString(e.what()) + " occurred");
        //go back to normal.
    }


*/

}

