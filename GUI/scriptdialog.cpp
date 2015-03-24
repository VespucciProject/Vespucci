#include "GUI/scriptdialog.h"
#include "ui_scriptdialog.h"
#include "External/Octave/VespucciOctave.h"
#include "External/R/VespucciR.h"

ScriptDialog::ScriptDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::ScriptDialog)
{
    ui->setupUi(this);
    dataset_ = ws->DatasetAt(row);
    code_box_ = this->findChild<QPlainTextEdit *>("codePlainTextEdit");
    receive_box_ = this->findChild<QPlainTextEdit *>("receivePlainTextEdit");
    send_box_ = this->findChild<QPlainTextEdit *>("sendPlainTextEdit");
    interpreter_selector_ = this->findChild<QComboBox *>("interpreterComboBox");
}

ScriptDialog::~ScriptDialog()
{
    delete ui;
}

void ScriptDialog::on_buttonBox_accepted()
{
    //Parse input
    std::string cmd = code_box_->document()->toPlainText().toStdString();
    QStringList invars = send_box_->document()->toPlainText().split("\n");
    QStringList outvars = receive_box_->document()->toPlainText().split("\n");

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
    if (interpreter_selector_->currentText() == "R"){
        int argc = 1;
        char* argv[1];
        argv[0] = "vespucci";
        VespucciR *R_instance = new VespucciR(argc, argv);
        //send variables to R
        R_instance->SetEnvironment(in_data);
        //Run R code
        R_instance->RunScript(cmd);
        //retrieve output from R
        data = R_instance->GetEnvironment(variable_keys);

        dataset_->AddAnalysisResults(data);
        //delete object created with "new"
        delete R_instance;

    }
    else if (interpreter_selector_->currentText() == "Octave"){
        //Do Octave stuff
    }
    else{
        QMessageBox::information(this, "Unknown or Unsupported Interpreter", "This interpreter is not usable from Vespucci", QMessageBox::Close);
    }




}

