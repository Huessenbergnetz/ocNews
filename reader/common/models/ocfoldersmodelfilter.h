#ifndef OCFOLDERSMODELFILTER_H
#define OCFOLDERSMODELFILTER_H

#include <QSortFilterProxyModel>

class OcFoldersModelFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit OcFoldersModelFilter(QObject *parent = 0);

signals:

public slots:

};

#endif // OCFOLDERSMODELFILTER_H
