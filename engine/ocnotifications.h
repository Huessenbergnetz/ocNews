#ifndef OCNOTIFICATIONS_H
#define OCNOTIFICATIONS_H

#include <QObject>

#if defined(MEEGO_EDITION_HARMATTAN)
#include <MNotification>
#include <MRemoteAction>
#else
#include <mlite-global.h>
#include <mlite5/MNotification>
#include <mlite5/MRemoteAction>
#endif

class OcNotifications : public QObject
{
    Q_OBJECT
public:
    explicit OcNotifications(QObject *parent = 0);

    enum Type { Default, Error, Success };

    void showNotification(const QString &body, const QString &summary = "ocNews", const Type &notificationType = Default);

signals:

public slots:

private:
    MNotification *notification;

};

#endif // OCNOTIFICATIONS_H
