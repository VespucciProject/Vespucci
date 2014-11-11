#ifndef DATALISTMODEL_H
#define DATALISTMODEL_H

#include <QAbstractListModel>

class DataListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DataListModel(QObject *parent = 0);
    DataListModel(QList<VespucciDataset> *datasets, QObject *parent);

signals:

public slots:

};

#endif // DATALISTMODEL_H
