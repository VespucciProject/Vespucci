#include "spectrumviewer.h"
#include "ui_spectrumviewer.h"

SpectrumViewer::SpectrumViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpectrumViewer)
{
    ui->setupUi(this);
}

SpectrumViewer::~SpectrumViewer()
{
    delete ui;
}
