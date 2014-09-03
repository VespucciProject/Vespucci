#include "analysiswizard.h"
#include "ui_analysiswizard.h"

AnalysisWizard::AnalysisWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::AnalysisWizard)
{
    ui->setupUi(this);
}

AnalysisWizard::~AnalysisWizard()
{
    delete ui;
}
