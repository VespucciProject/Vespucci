#include <Global/libvespucci.h>

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDir>
#include <Math/VespucciMath.h>
#include <Data/Import/textimport.h>
#include <QString>
///
/// \brief main
/// \param argc
/// \param argv
/// \return
/// Options:
/// batchvca components
/// -i indir : perform VCA on datasets in this directory, rather than working directory
/// -o outdir
/// -f window_size : perform median filtering with specified window size
/// -b poly_order max_it threshold : perform IModPoly baseline correction
/// -n type : perform normalization (minmax, area, z, snv)
/// --filter, --directory, --baseline
int main(int argc, char *argv[])
{
    using namespace std;
    using namespace arma;
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("batchvca");
    QCoreApplication::setApplicationVersion("1.0.0");
    QCommandLineParser parser;
    parser.setApplicationDescription("Batch VCA");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("components",
                                 QCoreApplication::translate("main",
                                                             "VCA components to calculate."));

    QCommandLineOption indir_option(QStringList() << "-i" << "--indir",
                                    QCoreApplication::translate("main",
                                                                "Parse files from <directory>"),
                                    QCoreApplication::translate("main",
                                                                "directory"));

    QCommandLineOption outdir_option(QStringList() << "-o" << "--outdir",
                                     QCoreApplication::translate("main",
                                                                 "Save output files to <directory>."),
                                     QCoreApplication::translate("main", "directory"));

    QCommandLineOption filter_option(QStringList() << "-f" << "--filter",
                                     QCoreApplication::translate("main",
                                                                 "Perform median filtering with <window_size>"),
                                     QCoreApplication::translate("main",
                                                                 "window_size"));

    QCommandLineOption normalize_option(QStringList() << "-n" << "--normalize",
                                        QCoreApplication::translate("main",
                                                                    "Perform normalization using <method>."),
                                        QCoreApplication::translate("main",
                                                                    "method"));
    QCommandLineOption baseline_option(QStringList() << "-b" << "--baseline",
                                       QCoreApplication::translate("main",
                                                                   "Perform IModPoly baselinbe correction with <opts>"),
                                       QCoreApplication::translate("main",
                                                                   "opts"));


    parser.addOption(indir_option);
    parser.addOption(outdir_option);
    parser.addOption(filter_option);
    parser.addOption(normalize_option);
    parser.addOption(baseline_option);
    parser.process(app);

    const QStringList args = parser.positionalArguments();

    int components = args.first().toInt();

    bool normalize = parser.isSet(normalize_option);
    bool correct_bl = parser.isSet(baseline_option);
    bool filter = parser.isSet(filter_option);
    int window_size = (filter ? parser.value(filter_option).toInt() : 0);
    QString baseline_opts = (correct_bl ? parser.value(baseline_option) : "");
    QStringList baseline_opts_list;
    int max_it = 0;
    int poly_order = 0;
    double threshold = 0;
    if (correct_bl){
        baseline_opts.remove("(");
        baseline_opts.remove(")");
        baseline_opts_list = baseline_opts.split(",");
        poly_order = baseline_opts_list.at(0).toInt();
        max_it = baseline_opts_list.at(1).toInt();
        threshold = baseline_opts_list.at(2).toDouble();
    }
    QString normalize_method = (normalize ? parser.value(normalize_option) : "");
    QString in_dir_path = (parser.isSet(indir_option) ?
                      parser.value(indir_option) :
                      QDir::currentPath());

    QString out_dir_path = (parser.isSet(outdir_option) ?
                       parser.value(outdir_option) :
                       QDir::currentPath());

    QDir in_dir(in_dir_path);
    QStringList in_filenames = in_dir.entryList(QDir::Files);
    mat spectra;
    vec abscissa, x, y;
    foreach(QString filename, in_filenames){
        try{
            cout << "Loading " << filename.toStdString() << "." << endl;
            TextImport::ImportWideText(filename.toStdString(),
                                       spectra,
                                       abscissa,
                                       x, y, true);
            uvec em_indices;
            if (filter){
                spectra =
                        Vespucci::Math::Smoothing::MedianFilterMat(spectra,
                                                                   window_size);
            }
            if (correct_bl){
                for (uword i = 0; i < spectra.n_cols; ++i){
                    vec spectrum = spectra.col(i);
                    double err;
                    vec baseline, corrected;
                    Vespucci::Math::LinLeastSq::IModPoly(spectrum,
                                                         abscissa,
                                                         baseline,
                                                         corrected,
                                                         err,
                                                         poly_order,
                                                         max_it,
                                                         threshold);
                    spectra.col(i) = corrected;
                }
            }
            if (normalize){
                if (normalize_method == "minmax"){
                    for (uword i = 0; i < spectra.n_cols; ++i){
                        vec spectrum = spectra.col(i);
                        double min = spectrum.min();
                        spectrum = spectrum - min*ones(spectrum.n_rows);
                        double max = spectrum.max();
                        spectrum = spectrum / max;
                        spectra.col(i) = spectrum;
                    }
                }
                if (normalize_method == "area"){
                    rowvec sums = sum(spectra);
                    rowvec mins = min(spectra);
                    for (uword i = 0; i < spectra.n_cols; ++i){
                        vec spectrum = spectra.col(i);
                        spectrum = spectrum - mins(i);
                        spectrum = spectrum / sums(i);
                        spectra.col(i) = spectrum;
                    }
                }
                if (normalize_method == "z"){
                    spectra =
                            Vespucci::Math::Normalization::StandardScoreMat(spectra);
                }
                if (normalize_method == "snv"){
                    spectra =
                            Vespucci::Math::Normalization::SNVNorm(spectra, 0);
                }
            }
            mat endmembers, projection, abundances;
            Vespucci::Math::DimensionReduction::VCA(spectra,
                                                    components,
                                                    em_indices,
                                                    endmembers,
                                                    projection,
                                                    abundances);
            string trunk = QFileInfo(filename).baseName().toStdString();
            string path = out_dir_path.toStdString();
            endmembers.save(path + "/" + trunk + "_endmembers.csv", csv_ascii);
            projection.save(path + "/" + trunk + "_projection.csv", csv_ascii);
            abundances.save(path + "/" + trunk + "_abundances.csv", csv_ascii);
        }catch(exception e){
            cout << "exception occurred" << endl;
            cout << e.what();
            continue;
        }
    }

    return app.exec();
}
