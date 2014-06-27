#ifndef OCFOLDERSMODELNEW_H
#define OCFOLDERSMODELNEW_H

#include <QAbstractListModel>
#include <QSqlQuery>
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

private:
    QList<OcFolderObject*> m_items;

    bool m_active;
    int m_totalUnread;

    void clear();
    void init();

    int findIndex(const int &id) const;
    void queryAndSetTotalCount();

};

#endif // OCFOLDERSMODELNEW_H
