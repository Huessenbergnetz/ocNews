#ifndef OCITEMSMODELNEW_H
#define OCITEMSMODELNEW_H

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

#include "../common/dbus/interfaces/ocdbusinterface.h"

class OcItemObject;

class OcItemsModelNew : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString feedId READ feedId WRITE setFeedId NOTIFY feedIdChanged)

public:
    explicit OcItemsModelNew(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;

    QString feedId() const;

    static const int TitleRole;
    static const int ItemIdRole;
    static const int PubDateRole;
    static const int PubDateIntRole;
    static const int EnclosureLinkRole;
    static const int EnclosureMimeRole;
    static const int UnreadRole;
    static const int StarredRole;
    static const int UrlRole;
    static const int GuidHashRole;
    static const int ExcerptRole;
    static const int ImageRole;

signals:
    void feedIdChanged(const QString &nFeedId);

public slots:
    void setFeedId(const QString &nFeedId);

    void itemsMarked(const QStringList &ids, const QString &action);
    void itemsStarred(const QStringList &hashes, const QString &action);
    void feedMarkedRead(const QString &markedFeedId);
    void itemsUpdated(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted);

private:
    QList<OcItemObject*> m_items;

    void init();
    void clear();

    QString m_feedId;

    QString niceTime(const uint &t) const;
    QString prepareBody(const QString &b) const;

    OcDBusInterface config;

};

#endif // OCITEMSMODELNEW_H
