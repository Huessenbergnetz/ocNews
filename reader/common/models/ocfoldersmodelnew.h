#ifndef OCFOLDERSMODELNEW_H
#define OCFOLDERSMODELNEW_H

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include "../../../common/globals.h"

class OcFolderObject;

class OcFoldersModelNew : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(int totalUnread READ totalUnread WRITE setTotalUnread NOTIFY totalUnreadChanged)
public:
    explicit OcFoldersModelNew(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;

    static const int IdRole;
    static const int TypeRole;
    static const int TitleRole;
    static const int UnreadCountRole;
    static const int IconSourceRole;
    static const int IconWidthRole;
    static const int IconHeightRole;
    static const int FeedCountRole;

    bool active() const;
    int totalUnread() const;

signals:
    void activeChanged(const bool &nActive);
    void totalUnreadChanged(const int &nTotalUnread);

public slots:
    void setActive(const bool &nActive);
    void setTotalUnread(const int &nTotalUnread);

    void itemsUpdated(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted);
    void itemsStarred();
    void itemsMarked();
    void itemsMarkedAllRead();

    void feedsRequested(const QList<int> &updated, const QList<int> &newFeeds, const QList<int> &deleted);
    void feedCreated(const QString &name, const int &id);
    void feedDeleted(const int &id);
    void feedMarkedRead(const int &id);
    void feedMoved(const int &feedId, const int &folderId);
    void feedRenamed(const QString &newName, const int &feedId);

    void foldersRequested(const QList<int> &updated, const QList<int> &newFolders, const QList<int> &deleted);
    void folderCreated(const QString &foldername, const int &folderId);
    void folderDeleted(const int &id);
    void folderMarkedRead(const int &id);
    void folderRenamed(const QString &newName, const int &id);

    void databaseCleaned();

private:
    QList<OcFolderObject*> m_items;

    bool m_active;
    int m_totalUnread;

    void clear();
    void init();

    int findIndex(const int &id, const int &type) const;
    void queryAndSetTotalCount();

};

#endif // OCFOLDERSMODELNEW_H
