#ifndef ANALYSISWIZARD_H
#define ANALYSISWIZARD_H

#include <QWizard>

namespace Ui {
class AnalysisWizard;
}

///
/// \brief The AnalysisWizard class
/// A wizard that allows analysis of data from multiple datasets
class AnalysisWizard : public QWizard
{
    Q_OBJECT

public:
    explicit AnalysisWizard(QWidget *parent = 0);
    ~AnalysisWizard();

private:
    Ui::AnalysisWizard *ui;
};

#endif // ANALYSISWIZARD_H
