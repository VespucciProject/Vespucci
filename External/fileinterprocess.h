#ifndef FILEINTERPROCESS_H
#define FILEINTERPROCESS_H
#include <Math/VespucciMath.h>
#include <Global/vespucci.h>
#include <QProcess>
namespace Vespucci{
    namespace External{
        class FileInterprocess
        {
        public:
            FileInterprocess(arma::field<std::string> invar_names,
                             arma::field<arma::mat> invars,
                             arma::field<std::string> outvar_names);
            arma::field<arma::mat> outvars();
            int RunR(const std::string &script, const QString &R_HOME);
            int RunOctave(const std::string &script);
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
