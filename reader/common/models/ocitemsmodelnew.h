#ifndef OCITEMSMODELNEW_H
#define OCITEMSMODELNEW_H

#include <QAbstractListModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>

#include "ocmodelhelper.h"

class OcItemObject;

class OcItemsModelNew : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int feedId READ feedId WRITE setFeedId NOTIFY feedIdChanged)
    Q_PROPERTY(bool populating READ populating WRITE setPopulating NOTIFY populatingChanged)
    Q_PROPERTY(bool showExcerpts READ showExcerpts WRITE setShowExcerpts NOTIFY showExcerptsChanged)
    Q_PROPERTY(bool showImages READ showImages WRITE setShowImages NOTIFY showImagesChanged)
public:
    explicit OcItemsModelNew(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;

    int feedId() const;
    bool populating() const;
    bool showExcerpts() const;
    bool showImages() const;

    Q_INVOKABLE void startCleanUpTimer();

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
    void feedIdChanged(const int &nFeedId);
    void populatingChanged(const bool &nPopulating);
    void showExcerptsChanged(const bool &nShowExcerpts);
    void showImagesChanged(const bool &nShowImages);

public slots:
    void setFeedId(const int &nFeedId);
    void setPopulating(const bool &nPopulating);
    void setShowExcerpts(const bool &nShowExcerpts);
    void setShowImages(const bool &nShowImages);

    void itemsMarked(const QStringList &ids, const QString &action);
    void itemsStarred(const QStringList &hashes, const QString &action);
    void feedMarkedRead(const int &markedFeedId);
    void allMarkedRead();
    void itemsUpdated(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted);

private slots:
    void clear();

private:
    QList<OcItemObject*> m_items;

    void init();

    int m_feedId;
    bool m_populating;
    bool m_showExcerpts;
    bool m_showImages;

    QTimer *timer;

    OcModelHelper helper;

};

#endif // OCITEMSMODELNEW_H
