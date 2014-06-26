#ifndef OCCOMBINEDMODELNEW_H
#define OCCOMBINEDMODELNEW_H

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QDir>
#include "../../../common/globals.h"

class OcCombinedObject;

class OcCombinedModelNew : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
public:
    explicit OcCombinedModelNew(QObject *parent = 0);

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
    static const int FolderIdRole;
    static const int FolderNameRole;

    bool active() const;

signals:
    void activeChanged(const bool &nActive);

public slots:
    void clear();
    void init();

    void setActive(const bool &nActive);

    void itemsUpdated(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted);
    void itemsStarred();
    void itemsMarked();

    void feedsRequested(const QList<int> &updated, const QList<int> &newFeeds, const QList<int> &deleted);

private:
    QList<OcCombinedObject*> m_items;

    bool m_active;

};

#endif // OCCOMBINEDMODELNEW_H
