#ifndef OCSPECIALITEMSMODELNEW_H
#define OCSPECIALITEMSMODELNEW_H

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QStringList>
#include "ocmodelhelper.h"
#include "ocitemsquery.h"
#include "../../../common/globals.h"
#include "objects/ocitemobject.h"

//class OcItemObject;

class OcSpecialItemsModelNew : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(bool showExcerpts READ showExcerpts WRITE setShowExcerpts NOTIFY showExcerptsChanged)
    Q_PROPERTY(bool showImages READ showImages WRITE setShowImages NOTIFY showImagesChanged)
    Q_PROPERTY(bool populating READ populating WRITE setPopulating NOTIFY populatingChanged)
public:
    explicit OcSpecialItemsModelNew(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;

    Q_INVOKABLE void clear();

    int type() const;
    int id() const;
    bool showExcerpts() const;
    bool showImages() const;
    bool populating() const;

signals:
    void typeChanged(const int &nType);
    void idChanged(const int &nId);
    void showExcerptsChanged(const bool &nShowExcerpts);
    void showImagesChanged(const bool &nShowImages);
    void populatingChanged(const bool &nPopulating);

public slots:
    void setType(const int &nType);
    void setId(const int &nId);
    void setShowExcerpts(const bool &nShowExcerpts);
    void setShowImages(const bool &nShowImages);
    void setPopulating(const bool &nPopulating);

    void itemsMarked(const QStringList &ids, const QString &action);
    void itemsStarred(const QStringList &hashes, const QString &action);
    void folderMarkedRead(const int &markedFolderId);
    void allMarkedRead();
    void itemsUpdated(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted);

private slots:
    void gotItem(OcItemObject *item);
    void queryFinished();


private:
    void init();

    QList<OcItemObject*> m_items;

    int m_type;
    int m_id;
    bool m_showExcerpts;
    bool m_showImages;
    bool m_populating;

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
    static const int FeedIdRole;
    static const int FeedNameRole;
    static const int ExcerptRole;
    static const int ImageRole;

    OcModelHelper helper;

};

#endif // OCSPECIALITEMSMODELNEW_H
