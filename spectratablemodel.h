#ifndef SPECTRATABLEMODEL_H
#define SPECTRATABLEMODEL_H

#include <QAbstractTableModel>
#include "vespuccidataset.h"
class VespucciDataset;
using namespace arma;

class SpectraTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SpectraTableModel(QObject *parent, QSharedPointer<VespucciDataset> dataset);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void ShedRow(int row);
signals:
public slots:

private:
    vec *x_;
    vec *y_;
    QSharedPointer<VespucciDataset> dataset_;
};

#endif // SPECTRATABLEMODEL_H
