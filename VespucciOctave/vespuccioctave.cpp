#define BOOST_DATE_TIME_NO_LIB
#include <iostream>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/program_options.hpp>
#include <cstdlib>
#include <sstream>
#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/toplev.h>
#include <octave/symtab.h>
//#include "arrayinterface.h"
#include <armadillo>

Matrix BuildMatrix(arma::mat *arma_mat);
arma::mat BuildArmaMatrix(Matrix &oct_mat);
bool GetSharedMemory(std::vector<std::string> &invar_names,
                     std::vector<std::string> &outvar_names);
void WriteWrapperFunction(const std::vector<std::string> &invar_names,
                          const std::vector<std::string> &outvar_names);
bool RunScript();



///
/// \brief main
/// \param argc
/// \param argv
/// \return
/// This program is called from vespucci to handle output to GNU Octave. This
/// kludge is necessary because Octave ships only as 32-bit Windows binaries,
/// and a decision has been made to link dynamically to the user's installation
/// of R and Octave (the dll's are in the path at runtime). This program is to
/// be compiled using a 32-bit toolkit and linking to the .dll.a liboctave and
/// liboctinterp libraries. Boost IPC is used to facilitate communication between
/// processes.
///
/// The program is called thusly:
/// vespuccioctave cmd_key map_key obj_key
/// Where cmd_key is the key of the shared memory object associated with the command
/// string, a boost::interprocess::basic_string, map_key is the key of a shared
/// memory object (a boost::interprocess::map) containing the names of numerical
/// object vectors and their number of rows (so that the vectors may be
/// reconstructed as matrices), and obj_key is the key of the shared memory
/// object associated with the numerical objects (a
/// boost::interprocess::managed_shared_memory). Numerical objects are retrieved
/// by their keys from this object.
///
/// Vespucci, when compiled with a version of Armadillo with 64-bit index support
/// (automatically enabled when using C++11 support, which is not required for
/// Armadillo as used by Vespucci but is required to compile Vespucci due to the
/// use of initializer lists and lambdas inside the Math namespace) can support
/// datasets with more than 2 billion elements (assuming they can be loaded into
/// RAM, as Vespucci is not a "big data" program), this 32-bit program cannot.
/// An exception will be thrown by the vespucci process if a matrix with more than
/// 2,147,483,647 is tried to be sent to this process.
int main(int argc, char *argv[])
{
    if (argc != 4){
        return 1;
    }
    //using namespace std;
    namespace ipc = boost::interprocess;
    typedef ipc::managed_shared_memory::size_type size_type;

    ipc::managed_shared_memory script_mem(ipc::open_only, argv[1]);
    ipc::managed_shared_memory obj_name_mem(ipc::open_only, argv[2]);
    ipc::managed_shared_memory obj_mem(ipc::open_only, argv[3]);


    ipc::managed_shared_memory obj_mem;

    std::pair<std::string*, size_type> script_pair;
    script_pair = script_mem.find<std::string>("script");
    //dereference script to send to the octave libraries.
    std::string script = *script_pair.first;

    //parse the shared memory object containing invar and outvar names
    std::pair<std::vector<std::string>, size_type> invars_pair;
    std::pair<std::vector<std::string>, size_type> outvars_pair;
    invars_pair = obj_map_mem.find<std::vector<std::string>("in");
    outvars_pair = obj_map_mem.find<std::vector<std::string> >("out");

    std::vector<std::string> outvar_names = outvars_pair.first;
    std::vector<std::string> invar_names = invars_pair.first;

    //create our wrapper function:
    typedef std::vector<std::string>::iterator strvec_it;
    std::string function_decl = "function [";

    strvec_it it = outvar_names.begin();
    function_decl += *it;
    ++it;
    while (it != outvar_names.end()){
        function_decl += ", ";
        function_decl += *it;
        ++it;
    }

    function_decl += "] = vespucciOctaveFunction (";

    it = invar_names.begin();
    function_decl += *it;
    ++it;
    while(it != invar_names.end()){
        function_decl += ", ";
        funciton_decl += *it;
        ++it;
    }

    function_decl += ")";

    //write script to file
    std::ofstream script_file;
    script_file.open("vespucciOctaveFunction.m");
    script_file << function_decl << std::endl
                << script << std::endl << "endfunction";
    script_file.close();

    string_vector oct_argv(2);
    oct_argv(0) = "embedded";
    oct_argv(1) = "-q";
    octave_main(2, oct_argv.c_str_vec(), 1);

    octave_value_list oct_invars;
    octave_value_list oct_outvars;

    //add input variables to input variable list
    for (int i = 0; i < invar_names.size(); ++i){
        std::string name = invar_names[i];
        arma::mat *arma_mat = obj_mem.find<arma::mat>(name).first;
        Matrix oct_mat = BuildMatrix(arma_mat);
        oct_invars(i) = oct_mat;
    }
    int nargout = invar_names.size();
    oct_outvars = feval("vespucciOctaveFunction", oct_invars, nargout);



    //clean up octave environment
    clean_up_and_exit(0, true);
    return 0;
}

Matrix BuildMatrix(arma::mat *arma_mat)

{
    arma::uword m = arma_mat->n_rows;
    arma::uword n = arma_mat->n_cols;

    Matrix matrix(m, n);
    //load all elements into
    for (int j = 0; j < n; ++j)
        for (int i = 0; i < m; ++i)
            matrix(i, j) = arma_mat->at(i, j);
    return matrix;
}

arma::mat BuildArmaMatrix(Matrix &oct_mat)
{
    size_t size = oct_mat.rows() * oct_mat.cols();
    double* data_start = oct_mat.fortran_vec();
    double* data_end = data_start + (size / sizeof(double));
    arma::mat matrix(std::vector<double>(data_start, data_end));
    matrix.resize(oct_mat.rows(), oct_mat.cols());
    oct_mat.clear(); //release memory in oct_mat because its been copied
    return matrix;
}
