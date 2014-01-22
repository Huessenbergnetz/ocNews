#include "ocdbusproxy.h"
#include <QDebug>

OcDBusProxy::OcDBusProxy(QObject *parent) :
    QObject(parent)
{
}

void OcDBusProxy::addFeed(const QStringList &feedUrl)
{
    emit dbusAddFeed(feedUrl.at(0));
}

void OcDBusProxy::showContent(const QStringList &itemId)
{
    QString t_itemId = itemId.at(0);
    emit dbusShowContent(t_itemId.remove(QRegExp("ocnews://")));
}
