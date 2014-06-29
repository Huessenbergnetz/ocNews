#include "ocspecialitemsmodelquery.h"
#include "../../../common/globals.h"

OcSpecialItemsModelQuery::OcSpecialItemsModelQuery(QObject *parent) :
    QThread(parent)
{
}


void OcSpecialItemsModelQuery::startQuery(const int &id, const int &type, const bool &showImgs, const bool &showExcerpt)
{
    m_id = id;
    m_type = type;
    m_showImgs = showImgs;
    m_showExcerpt = showExcerpt;

    this->start(NormalPriority);

}



void OcSpecialItemsModelQuery::run()
{
//    QList<OcItemObject*> items;

    QString querystring;
    QSqlQuery query;

//    int length = 0;

//    switch (m_type) {
//    case 1:
//        querystring = QString("SELECT count (id) FROM items WHERE feedId IN (SELECT id FROM feeds WHERE folderId = %1)").arg(m_id);
//        break;
//    case 2:
//        querystring = QString("SELECT count (id) FROM items WHERE starred = %1").arg(SQL_TRUE);
//        break;
//    case 3:
//    default:
//        querystring = "SELECT count (id) FROM items";
//        break;
//    }

//    query.exec(querystring);

//    query.next();

//    length = query.value(0).toInt();


    querystring = "SELECT it.id, it.title, it.pubDate, it.enclosureLink, it.enclosureMime, it.unread, it.starred, it.url, it.guidHash, ";

    if (m_showExcerpt) {
        querystring.append("it.body AS excerpt, ");
    } else {
        querystring.append("'' AS excerpt, ");
    }

    if (m_showImgs) {
        querystring.append("(SELECT DISTINCT path FROM images WHERE parentId = it.id AND height > 50 ORDER BY width, height LIMIT 1) AS image, ");
    } else {
        querystring.append("'' AS image, ");
    }

     querystring.append("(SELECT title FROM feeds where id = it.feedId) as feedName, it.feedId ");


    switch (m_type) {
    case 1:
        querystring.append(QString("FROM items it WHERE feedId IN (SELECT id FROM feeds WHERE folderId = %1)").arg(m_id));
        break;
    case 2:
        querystring.append("FROM items it WHERE starred = ").append(SQL_TRUE);
        break;
    case 3:
    default:
        querystring.append("FROM items it");
        break;
    }

    querystring.append(" ORDER BY pubDate DESC");

    query.exec(querystring);

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
//        items.append(iobj);
        emit gotRecord(iobj);
    }
}
