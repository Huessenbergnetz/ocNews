#ifndef OCSPECIALITEMSMODELQUERY_H
#define OCSPECIALITEMSMODELQUERY_H

#include <QThread>
#include <QSqlQuery>
#include <QList>
#include "ocmodelhelper.h"
#include "objects/ocitemobject.h"

//class OcItemObject;

class OcSpecialItemsModelQuery : public QThread
{
    Q_OBJECT
public:
    explicit OcSpecialItemsModelQuery(QObject *parent = 0);
    void startQuery(const int &id, const int &type, const bool &showImgs, const bool &showExcerpt);

signals:
    void queryFinished();
    void gotRecord(OcItemObject *item);

public slots:

protected:
    void run();

private:
    int m_type;
    int m_id;
    bool m_showImgs;
    bool m_showExcerpt;

    OcModelHelper helper;

};

#endif // OCSPECIALITEMSMODELQUERY_H
