#ifndef OCDBUSPROXY_H
#define OCDBUSPROXY_H

#include <QObject>
#include <QString>
#include <QStringList>

class OcDBusProxy : public QObject
{
    Q_OBJECT
public:
    explicit OcDBusProxy(QObject *parent = 0);

public Q_SLOTS: // METHODS
    void addFeed(const QStringList &feedUrl);
    void showContent(const QStringList &itemId);
    
signals:
    void dbusAddFeed(const QString &feedUrl);
    void dbusShowContent(const QString &itemId);
    
public slots:
    
};

#endif // OCDBUSPROXY_H
