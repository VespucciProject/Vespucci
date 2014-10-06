#include "metadataset.h"

MetaDataset::MetaDataset()
{
}

MetaDataset::MetaDataset(QString name, 
                         MainWindow *main_window, 
                         QFile *log_file,
                         QString *directory, 
                         QString method, 
                         QList<QSharedPointer<VespucciDataset> > parent_datasets)
{
    non_spatial_ = true;
    
}
