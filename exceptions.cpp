#include "exceptions.h"

ConstructorException::ConstructorException(const std::string& what_arg)
    : std::runtime_error(what_arg)
{

}

ProcessingException::ProcessingException(const std::string &what_arg)
    : std::runtime_error(what_arg)
{
}

ImagingException::ImagingException(const std::string &what_arg)
    : std::runtime_error(what_arg)
{
}


