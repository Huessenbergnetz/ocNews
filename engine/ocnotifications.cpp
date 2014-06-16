#include "ocnotifications.h"
#include <QDebug>

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
    MRemoteAction action("de.buschmann23.ocNewsReader", "/", "de.buschmann23.ocNewsReader", "", QVariantList() << QString());

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

    switch (notificationType)
    {
    case Success:
        type = "harbour.ocnews.success";
        image = "/usr/share/harbour-ocnews-reader/icons/harbour-ocnews-popup-success.png";
        break;
    case Error:
        type = "harbour.ocnews.error";
        image = "/usr/share/harbour-ocnews-reader/icons/harbour-ocnews-popup-error.png";
        break;
    case Default:
    default:
        type = "harbour.ocnews.notification";
        image = "/usr/share/harbour-ocnews-reader/icons/harbour-ocnews-popup.png";
        break;
    }

#endif

    notification = new MNotification(type, QString("ocNews - ").append(summary), body);
    notification->setImage(image);
//    MRemoteAction action("harbour.mitakuuluu2.client", "/", "harbour.mitakuuluu2.client", "notificationCallback", QVariantList() << QString());
    notification->setAction(action);
    notification->publish();
}
