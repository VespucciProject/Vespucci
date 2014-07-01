#ifndef ARMA_EXT_H
#define ARMA_EXT_H
#include "armadillo"
#include <QApplication>
#include <QVector>
#include <QMessageBox>

using namespace arma;

namespace arma_ext
{
mat spdiags(mat B, QVector<int> d, int m, int n);
mat svds(mat X, int k, std::string form);
mat svds(mat X, int k);
mat svds(mat X, std::string form);
mat svds(mat X);
}

#endif // ARMA_EXT_H
