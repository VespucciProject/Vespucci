#ifndef VESPUCCITABLEMODEL_H
#define VESPUCCITABLEMODEL_H

#include <QAbstractTableModel>
#include "vespucciworkspace.h"

class VespucciTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit VespucciTableModel(QObject *parent, mat *input_data);
    VespucciTableModel(QObject *parent, QSharedPointer<SpecMap> dataset, QString type);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

signals:

public slots:

private:
    int rows_;
    int cols_;
    mat *data_;
    QSharedPointer<SpecMap> dataset_;
    bool composite_;
    bool spatial_only_;

};

#endif // VESPUCCITABLEMODEL_H
