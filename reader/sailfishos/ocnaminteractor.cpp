#include "ocnaminteractor.h"
#include <QDebug>

OcNamInteractor::OcNamInteractor(QQmlEngine *qmlEngine, QObject *parent) :
    QObject(parent),
    m_qmlengine(qmlEngine)
{
}


void OcNamInteractor::clearCookies()
{
    QNetworkAccessManager * nam = m_qmlengine->networkAccessManager();
    if (nam) {
        QNetworkCookieJar * emptyCookieJar = new QNetworkCookieJar();
        // QNetworkAccessManager takes ownership of the cookieJar object.
        nam->setCookieJar(emptyCookieJar);
    }
}
