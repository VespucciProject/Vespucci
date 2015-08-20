#include <QCoreApplication>
#include <mlpack/core.hpp>
#include <boost/test/framework.hpp>


using namespace std;
using namespace arma;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    mat data;
    bool ok = data.load("testdata.csv");
    if (!ok){return 1;}
    return a.exec();
}
