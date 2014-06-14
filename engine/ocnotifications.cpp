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

    notification = new MNotification(type, QString("ocNews - ").append(summary), body);
    notification->setImage(image);
//    MRemoteAction action("harbour.mitakuuluu2.client", "/", "harbour.mitakuuluu2.client", "notificationCallback", QVariantList() << QString());
//    notification->setAction(action);
    notification->publish();
}
