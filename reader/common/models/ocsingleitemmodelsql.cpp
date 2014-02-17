#include "ocsingleitemmodelsql.h"


OcSingleItemModelSql::OcSingleItemModelSql(QObject *parent) :
    QObject(parent)
{
}


QVariantMap OcSingleItemModelSql::getItemData(const QString &itemId, bool showImg)
{
    QVariantMap itemresult;
    QSqlQuery query;
    query.exec(QString("SELECT it.id, it.guidHash, it.url, it.title, it.author, it.pubDate, it.body, it.enclosureMime, it.enclosureLink, it.unread, it.starred, it.feedId, "
                       "(SELECT title FROM feeds WHERE id = it.feedId) AS feedName, "
                       "IFNULL((SELECT id FROM items WHERE pubDate > it.pubDate AND feedId = it.feedId ORDER BY pubDate ASC LIMIT 1),0) AS previous, "
                       "IFNULL((SELECT id FROM items WHERE pubDate < it.pubDate AND feedId = it.feedId ORDER BY pubDate DESC LIMIT 1),0) AS next "
                       "FROM items it WHERE id = %1").arg(itemId.toInt()));

    if (query.next())
    {
        itemresult["id"] = query.value(0).toInt();
        itemresult["guidHash"] = query.value(1).toString();
        itemresult["url"] = query.value(2).toString();
        itemresult["title"] = query.value(3).toString();
        itemresult["author"] = query.value(4).toString();
        QString date = (QDateTime::currentDateTimeUtc().date() == QDateTime::fromTime_t(query.value(5).toUInt()).date()) ?
                        tr("Today") :
                        QDateTime::fromTime_t(query.value(5).toUInt()).toLocalTime().toString(tr("d. MMMM"));
        QString time = QDateTime::fromTime_t(query.value(5).toUInt()).toLocalTime().toString(tr("hh:mm"));
        itemresult["pubDate"] = date + " | " + time;
        itemresult["body"] = showImg ? query.value(6).toString() : query.value(6).toString().remove(QRegExp("<img[^>]*>"));

        itemresult["body"] = itemresult["body"].toString().remove(QRegExp("class=\".*\"\s"));

        itemresult["enclosureMime"] = query.value(7).toString();
        itemresult["enclosureLink"] = query.value(8).toString();
        itemresult["unread"] = query.value(9).toBool();
        itemresult["starred"] = query.value(10).toBool();
        itemresult["feedId"] = query.value(11).toString();
        itemresult["feedName"] = query.value(12).toString();
        itemresult["previous"] = query.value(13).toString();
        itemresult["next"] = query.value(14).toString();
        itemresult["containsImg"] = showImg ? false : query.value(6).toString().contains(QRegExp("<img[^>]*>"));
        if (itemresult["enclosureLink"] == "") {
            itemresult["enclosureType"] = 0;
            itemresult["enclosureHost"] = "";
            itemresult["enclosureName"] = "";
        } else {
            itemresult["enclosureType"] = getEnclosureType(query.value(7).toString());
            itemresult["enclosureHost"] = QUrl(query.value(8).toString()).host();
            itemresult["enclosureName"] = QFileInfo(query.value(8).toString()).fileName();
        }
    }

    return itemresult;
}


int OcSingleItemModelSql::getEnclosureType(const QString &encMime)
{

    if (encMime.contains("audio", Qt::CaseInsensitive)) {
        return 1;
    } else if (encMime.contains("video", Qt::CaseInsensitive)) {
        return 2;
    } else if (encMime.contains("pdf", Qt::CaseInsensitive)) {
        return 3;
    } else if (encMime.contains("image", Qt::CaseInsensitive)) {
        return 4;
    } else {
        return 0;
    }


}
