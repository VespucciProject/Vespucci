#ifndef INTERPROCESSSENDER_H
#define INTERPROCESSSENDER_H
#include <Math/VespucciMath.h>

namespace Vespucci{
    namespace External{
        class InterprocessSender
        {
        public:
            InterprocessSender(std::map<std::string, arma::mat*> invars, std::vector<std::string> outvar_names);
            bool SendToOctave(std::string command);
            bool SendToR(std::string command);
            std::map<std::string, arma::mat> GetValues();
        private:
            std::map<std::string, arma::mat*> invars_;
            std::vector<std::string> outvar_names_;
            std::map<std::string, arma::mat> outvars_;
            bool ReceiveValues();
        };

        class data_object
        {
            int n_rows_;
            int n_cols_;
            double_vector data_;
            char_string name_;
        public:
            data_object(const char* name, int n_rows, int n_cols, const void_allocator &void_alloc)
                : n_rows_(n_rows), n_cols_(n_cols), data_(void_alloc), name_(name, void_alloc){}
            void push_back(double val){data_.push_back(val);}
        };

    }
}

#endif // INTERPROCESSSENDER_H
