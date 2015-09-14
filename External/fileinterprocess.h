#ifndef FILEINTERPROCESS_H
#define FILEINTERPROCESS_H
#include <mlpack/core.hpp>
#include <QProcess>
namespace Vespucci{
    namespace External{
        class FileInterprocess
        {
        public:
            FileInterprocess(const arma::field<std::string> invar_names,
                             const arma::field<arma::mat> invars,
                             const arma::field<std::string> outvar_names);
            ~FileInterprocess();
            arma::field<arma::mat> outvars();
            int RunR(const std::string &script, const QString &R_HOME);
            int RunOctave(const std::string &script);
        public slots:
            void ProcessFinished();
        private:
            arma::field<std::string> invar_names_;
            arma::field<arma::mat> invars_;
            arma::field<std::string> outvar_names_;
            arma::field<arma::mat> outvars_;
            void GetOutvars();
            void DeleteTemporaryFiles();
        };
    }
}


#endif // FILEINTERPROCESS_H
