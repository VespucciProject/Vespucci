#ifndef PRINCIPALCOMPONENTSWORKER_H
#define PRINCIPALCOMPONENTSWORKER_H

#include <QObject>
#include "armadillo"
using namespace arma;
//a class that is a worker object for a QThread to perform principal component analysis
class PrincipalComponentsWorker : public QObject
{
    Q_OBJECT

public:
    PrincipalComponentsWorker(mat coeff, mat score, vec latent, vec tsquared, mat X);
    ~PrincipalComponentsWorker();

public slots:
    void princomp();

signals:
    void finished();
    void error(QString err);

private:
    mat coeff_;
    mat score_;
    vec latent_;
    vec tsquared_;
    mat X_;
};

#endif // PRINCIPALCOMPONENTSWORKER_H
