#ifndef DATASETLISTMODEL_H
#define DATASETLISTMODEL_H

#include <QAbstractListModel>
#include "vespucciworkspace.h"

class DatasetListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    DatasetListModel(QObject *parent, VespucciWorkspace *ws);
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
    int rowCount(const QModelIndex &parent) const;
    QSharedPointer<VespucciDataset> DatasetAt(const QModelIndex &parent);
    QList<QSharedPointer<VespucciDataset> > DatasetsAt(QList<QModelIndex> parents);

signals:

public slots:

private:
    QList<QSharedPointer<VespucciDataset> > datasets_;
};

#endif // DATASETLISTMODEL_H
