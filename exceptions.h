#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

class ConstructorException : public std::runtime_error
{
public:
    ConstructorException(const std::string &what_arg);
};

class ProcessingException : public std::runtime_error
{
public:
    ProcessingException(const std::string &what_arg);
};

class ImagingException : public std::runtime_error
{
public:
    ImagingException(const std::string &what_arg);
};

#endif // EXCEPTIONS_H
