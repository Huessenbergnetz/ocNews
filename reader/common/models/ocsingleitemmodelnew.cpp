#include "ocsingleitemmodelnew.h"
#include <QDebug>

OcSingleItemModelNew::OcSingleItemModelNew(QObject *parent) :
    QObject(parent)
{
    m_itemId = -1;
    m_handleRead = 0;
    m_enclosureType = 0;
    m_previous = 0;
    m_next = 0;
    m_parentFeedId = 0;
    m_sortAsc = false;
    m_unread = true;
    m_starred = false;
    m_containsImg = true;
    m_showImg = false;
    m_searchString = "";
    m_feedType = 0;
    m_guidHash = "";
    m_url = "";
    m_title = "";
    m_author = "";
    m_pubDate = "";
    m_body = "";
    m_enclosureMime = "";
    m_enclosureLink = "";
    m_feedName = "";
    m_feedId = 0;
    m_enclosureHost = "";
    m_enclosureName = "";
}


int OcSingleItemModelNew::itemId() const { return m_itemId; }

void OcSingleItemModelNew::setItemId(const int &nItemId)
{
    if (nItemId != m_itemId) {
        m_itemId = nItemId;
        get();
        emit itemIdChanged(itemId());
    }
}


int OcSingleItemModelNew::handleRead() const { return m_handleRead; }

void OcSingleItemModelNew::setHandleRead(const int &nHandleRead)
{
    if (nHandleRead != m_handleRead) {
        m_handleRead = nHandleRead;
        emit handleReadChanged(handleRead());
    }
}


int OcSingleItemModelNew::enclosureType() const { return m_enclosureType; }

void OcSingleItemModelNew::setEnclosureType(const int &nEnclosureType)
{
    if (nEnclosureType != m_enclosureType) {
        m_enclosureType = nEnclosureType;
        emit enclosureTypeChanged(enclosureType());
    }
}


int OcSingleItemModelNew::previous() const { return m_previous; }

void OcSingleItemModelNew::setPrevious(const int &nPrevious)
{
    if (nPrevious != m_previous) {
        m_previous = nPrevious;
        emit previousChanged(previous());
    }
}


int OcSingleItemModelNew::next() const { return m_next; }

void OcSingleItemModelNew::setNext(const int &nNext)
{
    if (nNext != m_next) {
        m_next = nNext;
        emit nextChanged(next());
    }
}


bool OcSingleItemModelNew::sortAsc() const { return m_sortAsc; }

void OcSingleItemModelNew::setSortAsc(const bool &nSortAsc)
{
    if (nSortAsc != m_sortAsc) {
        m_sortAsc = nSortAsc;
        emit sortAscChanged(sortAsc());
    }
}


bool OcSingleItemModelNew::unread() const { return m_unread; }

void OcSingleItemModelNew::setUnread(const bool &nUnread)
{
    if (nUnread != m_unread) {
        m_unread = nUnread;
        emit unreadChanged(unread());
    }
}


bool OcSingleItemModelNew::starred() const { return m_starred; }

void OcSingleItemModelNew::setStarred(const bool &nStarred)
{
    if (nStarred != m_starred) {
        m_starred = nStarred;
        emit starredChanged(starred());
    }
}


bool OcSingleItemModelNew::containsImg() const { return m_containsImg; }

void OcSingleItemModelNew::setContainsImg(const bool &nContainsImg)
{
    if (nContainsImg != m_containsImg) {
        m_containsImg = nContainsImg;
        emit containsImgChanged(containsImg());
    }
}


bool OcSingleItemModelNew::showImg() const { return m_showImg; }

void OcSingleItemModelNew::setShowImg(const bool &nShowImg)
{
    if (nShowImg != m_showImg) {
        m_showImg = nShowImg;
        get();
        emit showImgChanged(showImg());
    }
}


QString OcSingleItemModelNew::searchString() const { return m_searchString; }

void OcSingleItemModelNew::setSearchString(const QString &nSearchString)
{
    if (nSearchString != m_searchString) {
        m_searchString = nSearchString;
        emit searchStringChanged(searchString());
    }
}


int OcSingleItemModelNew::feedType() const { return m_feedType; }

void OcSingleItemModelNew::setFeedType(const int &nFeedType)
{
    if (nFeedType != m_feedType) {
        m_feedType = nFeedType;
        emit feedTypeChanged(feedType());
    }
}


int OcSingleItemModelNew::parentFeedId() const { return m_parentFeedId; }

void OcSingleItemModelNew::setParentFeedId(const int &nParentFeedId)
{
    if (nParentFeedId != m_parentFeedId) {
        m_parentFeedId = nParentFeedId;
        emit parentFeedIdChanged(parentFeedId());
    }
}


QString OcSingleItemModelNew::guidHash() const { return m_guidHash; }

void OcSingleItemModelNew::setGuidHash(const QString &nGuidHash)
{
    if (nGuidHash != m_guidHash) {
        m_guidHash = nGuidHash;
        emit guidHashChanged(guidHash());
    }
}


QString OcSingleItemModelNew::url() const { return m_url; }

void OcSingleItemModelNew::setUrl(const QString &nUrl)
{
    if (nUrl != m_url) {
        m_url = nUrl;
        emit urlChanged(url());
    }
}


QString OcSingleItemModelNew::title() const { return m_title; }

void OcSingleItemModelNew::setTitle(const QString &nTitle)
{
    if (nTitle != m_title) {
        m_title = nTitle;
        emit titleChanged(title());
    }
}


QString OcSingleItemModelNew::author() const { return m_author; }

void OcSingleItemModelNew::setAuthor(const QString &nAuthor)
{
    if (nAuthor != m_author) {
        m_author = nAuthor;
        emit authorChanged(author());
    }
}


QString OcSingleItemModelNew::pubDate() const { return m_pubDate; }

void OcSingleItemModelNew::setPubDate(const QString &nPubDate)
{
    if (nPubDate != m_pubDate) {
        m_pubDate = nPubDate;
        emit pubDateChanged(pubDate());
    }
}


QString OcSingleItemModelNew::body() const { return m_body; }

void OcSingleItemModelNew::setBody(const QString &nBody)
{
    if (nBody != m_body) {
        m_body = nBody;
        emit bodyChanged(body());
    }
}


QString OcSingleItemModelNew::enclosureMime() const { return m_enclosureMime; }

void OcSingleItemModelNew::setEnclosureMime(const QString &nEnclosureMime)
{
    if (nEnclosureMime != m_enclosureMime) {
        m_enclosureMime = nEnclosureMime;
        emit enclosureMimeChanged(enclosureMime());
    }
}


QString OcSingleItemModelNew::enclosureLink() const { return m_enclosureLink; }

void OcSingleItemModelNew::setEnclosureLink(const QString &nEnclosureLink)
{
    if (nEnclosureLink != m_enclosureLink) {
        m_enclosureLink = nEnclosureLink;
        emit enclosureLinkChanged(enclosureLink());
    }
}


QString OcSingleItemModelNew::feedName() const { return m_feedName; }

void OcSingleItemModelNew::setFeedName(const QString &nFeedName)
{
    if (nFeedName != m_feedName) {
        m_feedName = nFeedName;
        emit feedNameChanged(feedName());
    }
}


int OcSingleItemModelNew::feedId() const { return m_feedId; }

void OcSingleItemModelNew::setFeedId(const int &nFeedId)
{
    if (nFeedId != m_feedId) {
        m_feedId = nFeedId;
        emit feedIdChanged(feedId());
    }
}


QString OcSingleItemModelNew::enclosureHost() const { return m_enclosureHost; }

void OcSingleItemModelNew::setEnclosureHost(const QString &nEnclosureHost)
{
    if (nEnclosureHost != m_enclosureHost) {
        m_enclosureHost = nEnclosureHost;
        emit enclosureHostChanged(enclosureHost());
    }
}


QString OcSingleItemModelNew::enclosureName() const { return m_enclosureName; }

void OcSingleItemModelNew::setEnclosureName(const QString &nEnclosureName)
{
    if (nEnclosureName != m_enclosureName) {
        m_enclosureName = nEnclosureName;
        emit enclosureNameChanged(enclosureName());
    }
}



void OcSingleItemModelNew::get()
{
    QSqlQuery query;

    QString queryString = "SELECT it.id, it.guidHash, it.url, it.title, it.author, it.pubDate, it.body, it.enclosureMime, it.enclosureLink, it.unread, it.starred, it.feedId, "
                          "(SELECT title FROM feeds WHERE id = it.feedId) AS feedName, ";


    // get previous item based on sorting options

    queryString.append("IFNULL((SELECT id FROM items ");

    if (!sortAsc()) {
        queryString.append("WHERE pubDate > it.pubDate ");
    } else {
        queryString.append("WHERE pubDate < it.pubDate ");
    }

    if (feedType() == 0) {
        queryString.append(QString("AND feedId = %1 ").arg(parentFeedId()));
    } else if (feedType() == 1) {
        queryString.append(QString("AND feedId IN (SELECT id FROM feeds WHERE folderId = %1) ").arg(parentFeedId()));
    } else if (feedType() == 2) {
        queryString.append(QString("AND starred = %1 ").arg(SQL_TRUE));
    }

    if (handleRead() == 1)
        queryString.append(QString("AND unread = %1 ").arg(SQL_TRUE));

    if (!searchString().isEmpty())
    {
        QString t_search = searchString();
        t_search.prepend("%");
        t_search.append("%");
        queryString.append(QString(" AND title LIKE \"%1\"").arg(t_search));
    }


    queryString.append("ORDER BY ");

    if (handleRead() == 2)
        queryString.append("unread DESC, ");

    if (!sortAsc()) {
        queryString.append("pubDate ASC ");
    } else {
        queryString.append("pubDate DESC ");
    }

    queryString.append("LIMIT 1),0) AS previous, ");



    // get next item based on sorting options

    queryString.append("IFNULL((SELECT id FROM items ");

    if (!sortAsc()) {
        queryString.append("WHERE pubDate < it.pubDate ");
    } else {
        queryString.append("WHERE pubDate > it.pubDate ");
    }

    if (feedType() == 0) {
        queryString.append(QString("AND feedId = %1 ").arg(parentFeedId()));
    } else if (feedType() == 1) {
        queryString.append(QString("AND feedId IN (SELECT id FROM feeds WHERE folderId = %1) ").arg(parentFeedId()));
    } else if (feedType() == 2) {
        queryString.append(QString("AND starred = %1 ").arg(SQL_TRUE));
    }

    if (handleRead() == 1)
        queryString.append(QString("AND unread = %1 ").arg(SQL_TRUE));

    if (!searchString().isEmpty())
    {
        QString t_search = searchString();
        t_search.prepend("%");
        t_search.append("%");
        queryString.append(QString(" AND title LIKE \"%1\"").arg(t_search));
    }


    queryString.append("ORDER BY ");

    if (handleRead() == 2)
        queryString.append("unread DESC, ");

    if (!sortAsc()) {
        queryString.append("pubDate DESC ");
    } else {
        queryString.append("pubDate ASC ");
    }

    queryString.append(" LIMIT 1),0) AS next ");

    queryString.append(QString("FROM items it WHERE id = %1").arg(itemId()));

    query.exec(queryString);


    if (query.next())
    {
        setItemId(query.value(0).toInt());
        setGuidHash(query.value(1).toString());
        setUrl(query.value(2).toString());
        setTitle(query.value(3).toString());
        setAuthor(query.value(4).toString());
        QString date = (QDateTime::currentDateTimeUtc().date() == QDateTime::fromTime_t(query.value(5).toUInt()).date()) ?
                        tr("Today") :
                        QDateTime::fromTime_t(query.value(5).toUInt()).toLocalTime().toString(tr("d. MMMM"));
        QString time = QDateTime::fromTime_t(query.value(5).toUInt()).toLocalTime().toString(tr("hh:mm"));
        setPubDate(date + " | " + time);
        setBody(cleanHTML(query.value(6).toString(), showImg()));
        setEnclosureMime(query.value(7).toString());
        setEnclosureLink(query.value(8).toString());
        setUnread(query.value(9).toBool());
        setStarred(query.value(10).toBool());
        setFeedId(query.value(11).toInt());
        setFeedName(query.value(12).toString());
        setPrevious(query.value(13).toInt());
        setNext(query.value(14).toInt());
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        setContainsImg(showImg() ? false : query.value(6).toString().contains(QRegularExpression("<img[^>]*>")));
#else
        setContainsImg(showImg() ? false : query.value(6).toString().contains(QRegExp("<img[^>]*>")));
#endif
        if (enclosureLink().isEmpty()) {
            setEnclosureType(0);
            setEnclosureHost("");
            setEnclosureName("");
        } else {
            setEnclosureType(getEnclosureType(query.value(7).toString()));
            setEnclosureHost(QUrl(query.value(8).toString()).host());
            setEnclosureName(QFileInfo(query.value(8).toString()).fileName());
        }
    }

#ifdef QT_DEBUG
    qDebug() << "Item ID: " << itemId();
    qDebug() << "Show Img: " << showImg();
    qDebug() << "Handle read: " << handleRead();
    qDebug() << "Sort ascending: " << sortAsc();
    qDebug() << "Search string: " << searchString();
    qDebug() << "Parent Feed type: " << feedType();
    qDebug() << "Parent Feed id: " << parentFeedId();
    qDebug() << "Next id: " << next();
    qDebug() << "Previous id: " << previous();
#endif
}


int OcSingleItemModelNew::getEnclosureType(const QString &encMime)
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


QString OcSingleItemModelNew::cleanHTML(const QString &html, bool showImg)
{
    QString m_html = html;

    if (!showImg) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        m_html.remove(QRegularExpression("<img[^>]*>"));
        m_html.remove(QRegularExpression("<a[^>]*>\\s*</a>"));
#else
        m_html.remove(QRegExp("<img[^>]*>"));
        m_html.remove(QRegExp("<a[^>]*>\\s*</a>"));
#endif
    }

//    m_html.remove(QRegExp("class=\"[\\w\\s-]*\"")); // remove class statements

    return m_html;
}



void OcSingleItemModelNew::showImages()
{
    QSqlQuery q;

    q.exec(QString("SELECT body FROM items WHERE id = %1").arg(itemId()));

    q.next();

    setBody(q.value(0).toString());
}
