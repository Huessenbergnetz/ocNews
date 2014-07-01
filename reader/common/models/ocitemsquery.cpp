#include "ocitemsquery.h"

OcItemsQuery::OcItemsQuery(QObject *parent) :
    QThread(parent)
{
}



void OcItemsQuery::startQuery(const QString queryString, const bool &specialItemsModel)
{
    m_queryString = queryString;
    m_specialItemsModel = specialItemsModel;

    this->start(NormalPriority);

}



void OcItemsQuery::run()
{

    QSqlQuery query;

    query.exec(m_queryString);

    if (m_specialItemsModel) {
        while(query.next())
        {
            OcItemObject *iobj = new OcItemObject(query.value(0).toInt(),
                                                  query.value(1).toString(),
                                                  query.value(2).toUInt(),
                                                  query.value(3).toString(),
                                                  query.value(4).toString(),
                                                  query.value(5).toBool(),
                                                  query.value(6).toBool(),
                                                  query.value(7).toString(),
                                                  query.value(8).toString(),
                                                  helper.prepareBody(query.value(9).toString()),
                                                  query.value(10).toString(),
                                                  query.value(11).toString(),
                                                  query.value(12).toInt());
            emit gotRecord(iobj);
        }
    } else {
        while(query.next())
        {
            OcItemObject *iobj = new OcItemObject(query.value(0).toInt(),
                                                  query.value(1).toString(),
                                                  query.value(2).toUInt(),
                                                  query.value(3).toString(),
                                                  query.value(4).toString(),
                                                  query.value(5).toBool(),
                                                  query.value(6).toBool(),
                                                  query.value(7).toString(),
                                                  query.value(8).toString(),
                                                  helper.prepareBody(query.value(9).toString()),
                                                  query.value(10).toString(),
                                                  "",
                                                  -1);
            emit gotRecord(iobj);
        }
    }
}
