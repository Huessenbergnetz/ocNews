#include "ocnotifications.h"
#include "QsLog.h"

OcNotifications::OcNotifications(QObject *parent) :
    QObject(parent)
{
    notification = 0;
}


void OcNotifications::showNotification(const QString &body, const QString &summary, const Type &notificationType)
{
    if (notification)
    {
        if (notification->isPublished()) {
            notification->remove();
        }

        notification = 0;
    }

    QString type = "";
    QString image = "";

#if defined(MEEGO_EDITION_HARMATTAN)

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

#else

    MRemoteAction action("harbour.ocnews.reader", "/", "harbour.ocnews.reader", "activate");

    switch (notificationType)
    {
    case Success:
        type = MNotification::TransferCompleteEvent;
        image = "/usr/share/harbour-ocnews-reader/icons/harbour-ocnews-popup-success.png";
        break;
    case Error:
        type = MNotification::TransferErrorEvent;
        image = "/usr/share/harbour-ocnews-reader/icons/harbour-ocnews-popup-error.png";
        break;
    case Default:
    default:
        type = MNotification::TransferEvent;
        image = "/usr/share/harbour-ocnews-reader/icons/harbour-ocnews-popup.png";
        break;
    }

#endif

    notification = new MNotification(type, QString("ocNews - ").append(summary), body);
    notification->setImage(image);
    notification->setAction(action);
    if (!notification->publish()) {
        QLOG_ERROR() << "Notifications: publishing failed.";
    }
}
