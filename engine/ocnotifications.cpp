#include "ocnotifications.h"
#include "QsLog.h"

OcNotifications::OcNotifications(QObject *parent) :
    QObject(parent)
{
#if defined(MEEGO_EDITION_HARMATTAN)
    notification = 0;
#else
    notification = nullptr;
#endif
}


void OcNotifications::showNotification(const QString &body, const QString &summary, const Type &notificationType)
{
#if defined(MEEGO_EDITION_HARMATTAN)

    if (notification)
    {
        if (notification->isPublished()) {
            notification->remove();
        }

        notification = 0;
    }

    QString type = "";
    QString image = "";


    image = "/usr/share/themes/blanco/meegotouch/icons/icon-m-ocnews.png";
    MRemoteAction action("de.buschmann23.ocNewsReader", "/", "de.buschmann23.ocNewsReader", "activate");

    switch (notificationType)
    {
    case Success:
        type = MNotification::TransferCompleteEvent;
        break;
    case Error:
        type = MNotification::TransferErrorEvent;
        break;
    case Default:
    default:
        type = MNotification::TransferEvent;
        break;
    }

    notification = new MNotification(type, QString("ocNews - ").append(summary), body);

    notification->setImage(image);
    notification->setAction(action);
    if (!notification->publish()) {
        QLOG_ERROR() << "Notifications: publishing failed.";
    }

#else

    notification = new Notification;

    notification->setAppName(QStringLiteral("ocNews"));
    notification->setPreviewBody(body);
    notification->setPreviewSummary(summary);
    notification->setBody(body);
    notification->setSummary(summary);
    notification->setRemoteAction(Notification::remoteAction(QStringLiteral("default"), QString(), QStringLiteral("harbour.ocnews.reader"), QStringLiteral("/"), QStringLiteral("harbour.ocnews.reader"), QStringLiteral("activate")));

    switch (notificationType) {
    case Success:
        notification->setAppIcon(QStringLiteral("/usr/share/harbour-ocnews-reader/icons/harbour-ocnews-popup-success.png"));
        notification->setUrgency(Notification::Normal);
        break;
    case Error:
        notification->setAppIcon(QStringLiteral("usr/share/harbour-ocnews-reader/icons/harbour-ocnews-popup-error.png"));
        notification->setUrgency(Notification::Critical);
        break;
    case Default:
    default:
        notification->setAppIcon(QStringLiteral("/usr/share/harbour-ocnews-reader/icons/harbour-ocnews-popup.png"));
        notification->setUrgency(Notification::Low);
        break;
    }

    notification->publish();

#endif

}
