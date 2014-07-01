#ifndef OCITEMSQUERY_H
#define OCITEMSQUERY_H

#include <QThread>
#include <QSqlQuery>
#include <QList>
#include <QVariant>
#include "ocmodelhelper.h"
#include "objects/ocitemobject.h"

class OcItemsQuery : public QThread
{
    Q_OBJECT
public:
    explicit OcItemsQuery(QObject *parent = 0);
    void startQuery(const QString queryString, const bool &specialItemsModel);

signals:
    void gotRecord(OcItemObject *item);

public slots:

protected:
    void run();

private:
    QString m_queryString;
    bool m_specialItemsModel;

    OcModelHelper helper;

};

#endif // OCITEMSQUERY_H
