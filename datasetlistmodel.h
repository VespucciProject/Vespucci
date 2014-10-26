#ifndef DATASETLISTMODEL_H
#define DATASETLISTMODEL_H

#include <QAbstractListModel>
#include "vespucciworkspace.h"

class VespucciWorkspace;

///
/// \brief The DatasetListModel class Exposes the UI to the contents of the master dataset list
///
class DatasetListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    DatasetListModel(QObject *parent, VespucciWorkspace *ws);
    int rowCount(const QModelIndex &parent) const;
    bool removeRow(int row, const QModelIndex &parent);
    bool AddDataset(QSharedPointer<VespucciDataset> dataset);
    QSharedPointer<VespucciDataset> DatasetAt(int row);
    QVariant data(const QModelIndex &index, int role) const;
    void ClearDatasets();

signals:
    void DatasetAdded(const QModelIndex &index);

public slots:

private:
    //This view displays the names of datasets stored
    ///
    /// \brief datasets_ The master dataset list in the workspace
    ///
    QList<QSharedPointer<VespucciDataset> > datasets_;
    VespucciWorkspace *workspace;
};

#endif // DATASETLISTMODEL_H
