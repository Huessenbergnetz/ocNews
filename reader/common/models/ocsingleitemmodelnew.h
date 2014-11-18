#ifndef OCSINGLEITEMMODELNEW_H
#define OCSINGLEITEMMODELNEW_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QUrl>
#include <QFileInfo>
#include <QDateTime>
#include <QVariant>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QRegularExpression>
#else
#include <QRegExp>
#endif
#include "../../../common/globals.h"

class OcSingleItemModelNew : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int itemId READ itemId WRITE setItemId NOTIFY itemIdChanged)
    Q_PROPERTY(int handleRead READ handleRead WRITE setHandleRead NOTIFY handleReadChanged)
    Q_PROPERTY(int enclosureType READ enclosureType WRITE setEnclosureType NOTIFY enclosureTypeChanged)
    Q_PROPERTY(int previous READ previous WRITE setPrevious NOTIFY previousChanged)
    Q_PROPERTY(int next READ next WRITE setNext NOTIFY nextChanged)
    Q_PROPERTY(int parentFeedId READ parentFeedId WRITE setParentFeedId NOTIFY parentFeedIdChanged)
    Q_PROPERTY(bool sortAsc READ sortAsc WRITE setSortAsc NOTIFY sortAscChanged)
    Q_PROPERTY(bool unread READ unread WRITE setUnread NOTIFY unreadChanged)
    Q_PROPERTY(bool starred READ starred WRITE setStarred NOTIFY starredChanged)
    Q_PROPERTY(bool containsImg READ containsImg WRITE setContainsImg NOTIFY containsImgChanged)
    Q_PROPERTY(bool showImg READ showImg WRITE setShowImg NOTIFY showImgChanged)
    Q_PROPERTY(QString searchString READ searchString WRITE setSearchString NOTIFY searchStringChanged)
    Q_PROPERTY(int feedType READ feedType WRITE setFeedType NOTIFY feedTypeChanged)
    Q_PROPERTY(QString guidHash READ guidHash WRITE setGuidHash NOTIFY guidHashChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
    Q_PROPERTY(QString pubDate READ pubDate WRITE setPubDate NOTIFY pubDateChanged)
    Q_PROPERTY(QString body READ body WRITE setBody NOTIFY bodyChanged)
    Q_PROPERTY(QString enclosureMime READ enclosureMime WRITE setEnclosureMime NOTIFY enclosureMimeChanged)
    Q_PROPERTY(QString enclosureLink READ enclosureLink WRITE setEnclosureLink NOTIFY enclosureLinkChanged)
    Q_PROPERTY(QString feedName READ feedName WRITE setFeedName NOTIFY feedNameChanged)
    Q_PROPERTY(int feedId READ feedId WRITE setFeedId NOTIFY feedIdChanged)
    Q_PROPERTY(QString enclosureHost READ enclosureHost WRITE setEnclosureHost NOTIFY enclosureHostChanged)
    Q_PROPERTY(QString enclosureName READ enclosureName WRITE setEnclosureName NOTIFY enclosureNameChanged)
public:
    explicit OcSingleItemModelNew(QObject *parent = 0);

    Q_INVOKABLE void showImages();

    int itemId() const;
    int handleRead() const;
    int enclosureType() const;
    int parentFeedId() const;
    int previous() const;
    int next() const;
    bool sortAsc() const;
    bool unread() const;
    bool starred() const;
    bool containsImg() const;
    bool showImg() const;
    QString searchString() const;
    int feedType() const;
    QString guidHash() const;
    QString url() const;
    QString title() const;
    QString author() const;
    QString pubDate() const;
    QString body() const;
    QString enclosureMime() const;
    QString enclosureLink() const;
    QString feedName() const;
    int feedId() const;
    QString enclosureHost() const;
    QString enclosureName() const;

signals:
    void itemIdChanged(const int &nItemId);
    void handleReadChanged(const int &nHandleRead);
    void enclosureTypeChanged(const int &nEnclosureType);
    void parentFeedIdChanged(const int &nParentFeedId);
    void previousChanged(const int &nPrevious);
    void nextChanged(const int &nNext);
    void sortAscChanged(const bool &nSortAsc);
    void unreadChanged(const bool &nUnread);
    void starredChanged(const bool &nStarred);
    void containsImgChanged(const bool &nContainsImg);
    void showImgChanged(const bool &nShowImg);
    void searchStringChanged(const QString &nSearchString);
    void feedTypeChanged(const int &nFeedType);
    void guidHashChanged(const QString &nGuidHash);
    void urlChanged(const QString &nUrl);
    void titleChanged(const QString &nTitle);
    void authorChanged(const QString &nAuthor);
    void pubDateChanged(const QString &nPubDate);
    void bodyChanged(const QString &nBody);
    void enclosureMimeChanged(const QString &nEnclosureMime);
    void enclosureLinkChanged(const QString &nEnclosureLink);
    void feedNameChanged(const QString &nFeedName);
    void feedIdChanged(const int &nFeedId);
    void enclosureHostChanged(const QString &nEnclosureHost);
    void enclosureNameChanged(const QString &nEnclosureName);

public slots:
    void setItemId(const int &nItemId);
    void setHandleRead(const int &nHandleRead);
    void setEnclosureType(const int &nEnclosureType);
    void setParentFeedId(const int &nParentFeedId);
    void setPrevious(const int &nPrevious);
    void setNext(const int &nNext);
    void setSortAsc(const bool &nSortAsc);
    void setUnread(const bool &nUnread);
    void setStarred(const bool &nStarred);
    void setContainsImg(const bool &nContainsImg);
    void setShowImg(const bool &nShowImg);
    void setSearchString(const QString &nSearchString);
    void setFeedType(const int &nFeedType);
    void setGuidHash(const QString &nGuidHash);
    void setUrl(const QString &nUrl);
    void setTitle(const QString &nTitle);
    void setAuthor(const QString &nAuthor);
    void setPubDate(const QString &nPubDate);
    void setBody(const QString &nBody);
    void setEnclosureMime(const QString &nEnclosureMime);
    void setEnclosureLink(const QString &nEnclosureLink);
    void setFeedName(const QString &nFeedName);
    void setFeedId(const int &nFeedId);
    void setEnclosureHost(const QString &nEnclosureHost);
    void setEnclosureName(const QString &nEnclosureName);

private:
    int m_itemId;
    int m_handleRead;
    int m_enclosureType;
    int m_parentFeedId;
    int m_previous;
    int m_next;
    bool m_sortAsc;
    bool m_unread;
    bool m_starred;
    bool m_containsImg;
    bool m_showImg;
    QString m_searchString;
    int m_feedType;
    QString m_guidHash;
    QString m_url;
    QString m_title;
    QString m_author;
    QString m_pubDate;
    QString m_body;
    QString m_enclosureMime;
    QString m_enclosureLink;
    QString m_feedName;
    int m_feedId;
    QString m_enclosureHost;
    QString m_enclosureName;

    void get();
    int getEnclosureType(const QString &encMime);
    QString cleanHTML(const QString &html, bool showImg);

};

#endif // OCSINGLEITEMMODELNEW_H
