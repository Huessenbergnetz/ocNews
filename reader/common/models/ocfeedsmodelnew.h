#ifndef OCFEEDSMODELNEW_H
#define OCFEEDSMODELNEW_H

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QDir>
#include "../../../common/globals.h"

class OcFeedObject;

class OcFeedsModelNew : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int folderId READ folderId WRITE setFolderId NOTIFY folderIdChanged)
public:
    explicit OcFeedsModelNew(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;

    int folderId() const;

    static const int IdRole;
    static const int TypeRole;
    static const int TitleRole;
    static const int UnreadCountRole;
    static const int IconSourceRole;
    static const int IconWidthRole;
    static const int IconHeightRole;

signals:
    void folderIdChanged(const int &nFolderId);

public slots:
    void clear(const bool &resetId = true);
    void init();

    void setFolderId(const int &nFolderId);

    void itemsUpdated(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted);
    void itemsMarked();

    void feedsRequested(const QList<int> &updated, const QList<int> &newFeeds, const QList<int> &deleted);

private:
    int m_folderId;

    QList<OcFeedObject*> m_items;

    int findIndex(const int &id, const int &type) const;
    void queryAndSetTotalUnread();
};

#endif // OCFEEDSMODELNEW_H
