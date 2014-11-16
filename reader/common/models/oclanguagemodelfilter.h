#ifndef OCLANGUAGEMODELFILTER_H
#define OCLANGUAGEMODELFILTER_H

#include <QSortFilterProxyModel>
#include "oclanguagemodel.h"

class OcLanguageModelFilter : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString search READ search WRITE setSearch NOTIFY searchChanged)
public:
    explicit OcLanguageModelFilter(QObject *parent = 0);

    QString search() const;

    Q_INVOKABLE QString getLanguageName(const QString &langCode);

signals:
    void searchChanged(const QString &nSearch);

public slots:
    void setSearch(const QString &nSearch);

private:
    QString m_search;
    OcLanguageModel m_sourceModel;

};

#endif // OCLANGUAGEMODELFILTER_H
