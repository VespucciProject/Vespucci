#ifndef METADATASET_H
#define METADATASET_H
#include "vespuccidataset.h"
///
/// \brief The MetaDataset class
/// A subclass of VespucciDataset for datasets created from multiple other
/// datasets. this stores information about the parents.
class MetaDataset : public VespucciDataset
{
public:
    MetaDataset();
    MetaDataset(QString name, 
                MainWindow *main_window,
                QFile *log_file,
                QString *directory, 
                QString method,
                QList<QSharedPointer<VespucciDataset> > parent_datasets);
private:
    ///
    /// \brief parent_datasets_
    /// Pointers to the dataset that this dataset is constructed from
    QList<QSharedPointer<VespucciDataset> > parent_datasets_;
};

#endif // METADATASET_H
