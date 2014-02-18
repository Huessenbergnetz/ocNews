#ifndef OCSINGLEITEMMODELSQL_H
#define OCSINGLEITEMMODELSQL_H

#include <QObject>
#include <QtSql>
#include <QUrl>

class OcSingleItemModelSql : public QObject
{
    Q_OBJECT
public:
    explicit OcSingleItemModelSql(QObject *parent = 0);

    Q_INVOKABLE QVariantMap getItemData(const QString &itemId, bool showImg = false);
    
signals:
    
public slots:

private:
    int getEnclosureType(const QString &encMime);
    QString cleanHTML(const QString &html, bool showImg);

    
};

#endif // OCSINGLEITEMMODELSQL_H
