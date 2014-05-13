#ifndef SPECTRUMVIEWER_H
#define SPECTRUMVIEWER_H

#include <QDialog>

namespace Ui {
class SpectrumViewer;
}

class SpectrumViewer : public QDialog
{
    Q_OBJECT

public:
    explicit SpectrumViewer(QWidget *parent = 0);
    ~SpectrumViewer();

private:
    Ui::SpectrumViewer *ui;
};

#endif // SPECTRUMVIEWER_H
