#include "External/interprocesssender.h"


Vespucci::External::
InterprocessSender::InterprocessSender(std::map<std::string, arma::mat *> invars,
                                       std::vector<std::string> outvar_names)
    :invars_(invars), outvar_names(outvar_names)
{

}
