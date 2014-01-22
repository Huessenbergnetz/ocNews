#include <QDebug>
#include "ochelper.h"



/*!
 * \class OcHelper
 * \brief The OcHelper class provides some helper functions
 *
 * This class provides helper functions like building the API URL and
 * convert the JSON results into QVarianMap.
*/

OcHelper::OcHelper(QObject *parent) :
    QObject(parent)
{
}



/*!
 * \fn QString OcHelper::buildUrl(QString apipart)
 * \brief Return the API URL
 *
 * This function uses the stored server address and combines it with the
 * used API part and the used protocol to build the API URL.
 *
 * \param apipart       The part of the API that is used
 * \return              QString with the complete URL
 */

QString OcHelper::buildUrl(QString apipart)
{
    QString domain;
    QString url;
    QVariantMap account = config.getAccount();
    bool usessl = true;
    if (account["state"].toInt() == 0)
    {
        domain = account["server"].toString();
        usessl = account["usessl"].toBool();
    }

    if (domain.endsWith("/"))
        domain.chop(1);

    if (domain.startsWith("http://", Qt::CaseInsensitive))
        domain.remove(0, 7);

    if (domain.startsWith("https://", Qt::CaseInsensitive))
        domain.remove(0, 8);

    if (usessl)
    {
        url = "https://";
    } else {
        url = "http://";
    }
    url.append(domain);
    url.append("/index.php/apps/news/api/v1-2/");
    url.append(apipart);

#ifdef QT_DEBUG
    qDebug() << url;
#endif

    return url;
}




/*!
 * \fn QVariantMap OcHelper::jsonToQt(QByteArray json)
 * \brief Converts th JSON server reply into a QVariantMap
 *
 * This functions takes the server JSON reply as a QByteArray and
 * converts it into a QVariantMap.
 *
 * \param json      QByteArray that holds the JSON reply
 * \return          The reply as QVariantMap
 */

QVariantMap OcHelper::jsonToQt(QByteArray json)
{
    QVariantMap result;

#if defined(MEEGO_EDITION_HARMATTAN)

    QJson::Parser parser;
    bool ok;
    result = parser.parse (json, &ok).toMap();
    if (!ok) qFatal("An error occured during parsing of the JSON");

#else

    result = QJsonDocument::fromJson(json).object().toVariantMap();

#endif

    return result;
}
