#include "fileinterprocess.h"
#include <QDir>
#include <QDirIterator>
Vespucci::External::FileInterprocess::FileInterprocess(const arma::field<std::string> &invar_names,
                                                       const arma::field<arma::mat> &invars,
                                                       const arma::field<std::string> &outvar_names)
    : invar_names_(invar_names), invars_(invars), outvar_names_(outvar_names), outvars_(outvar_names.n_elem)
{
    invar_names_.save("interprocess/invar_names.tmp");
    invars_.save("interprocess/invars.tmp");
    outvar_names_.save("interprocess/outvar_names.tmp");
}


arma::field<arma::mat> Vespucci::External::FileInterprocess::outvars()
{
    return outvars_;
}

int Vespucci::External::FileInterprocess::RunR(const std::string &script, const QString &R_HOME)
{
    QProcess R_process();
    QStringList env_vars = QProcess::systemEnvironment();
    env_vars << "R_HOME="+R_HOME;//set R_HOME environment variable before starting process
    R_process.setEnvironment(env_vars);
    std::ofstream cmd_stream("interprocess/commands.tmp");
    cmd_stream << script;
    cmd_stream.close();

    QStringList args;
    args << "interprocess/invar_names.tmp"
         << "interprocess/invars.tmp"
         << "interprocess/outvar_names.tmp"
         << "interprocess/outvars.tmp"
         << "interprocess/commands.tmp";
    R_process.start("vespucciR", args);
    return R_process.exitCode();
}

int Vespucci::External::FileInterprocess::RunOctave(const std::string &script)
{
    QProcess Octave_process();
    QStringList env_vars = QProcess::systemEnvironment();
    Octave_process.setEnvironment(env_vars);
    std::ofstream cmd_stream("interprocess/commands_tmp.m");
    //create wrapper function for octave to call
    cmd_stream << "function ";
    if (outvar_names.n_elem){
        cmd_stream << "[";
        for (arma::uword i = 0; i < outvar_names_.n_elem - 1; ++i){
            cmd_stream << outvar_names_(i) << ",";
        }
        cmd_stream << outvar_names_(outvar_names.n_elem - 1) << "] = ";
    }
    cmd_stream << "VespucciOctave(";
    for (arma::uword i = 0; i < invar_names_.n_elem - 1; ++i){
        cmd_stream << invar_names_(i) << ",";
    }
    cmd_stream << invar_names(invar_names_.n_elem - 1) << ")" << std::endl;

    cmd_stream << script << std::endl << "endfunction";
    cmd_stream.close();

    QStringList args;
    args << "interprocess/invar_names.tmp"
         << "interprocess/invars.tmp"
         << "interprocess/outvar_names.tmp"
         << "interprocess/outvars.tmp"
         << "interprocess/commands_tmp.m";
    Octave_process.start("vespucciOctave", args);
    return Octave_process.exitCode();
}

void Vespucci::External::FileInterprocess::GetOutvars()
{
    outvars_.load("interprocess/outvars.tmp");
}

void Vespucci::External::FileInterprocess::DeleteTemporaryFiles()
{
    QDirIterator it("interprocess", QDirIterator::NoIteratorFlags);
    while(it.hasNext()){
        if (it.fileInfo().suffix() == "tmp" || it.fileInfo().suffix() == "m"){
            QDir::remove(it.filePath());
        }
    }
}
