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
 * \fn QUrl OcHelper::buildUrl(QString apipart)
 * \brief Return the API URL
 *
 * This function uses the stored server address and combines it with the
 * used API part and the used protocol to build the API URL.
 *
 * \param apipart       The part of the API that is used
 * \return              QUrl the complete URL
 */

QUrl OcHelper::buildUrl(QString apipart)
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

    return QUrl(url);
}


/*!
 * \fn QNetworkRequest OcHelper::buildRequest(const QUrl &url)
 * \brief Builds the authorization header value
 *
 * This function build the authorization header value.
 *
 * \param url QUrl the url that should be requested
 * \return
 */
QNetworkRequest OcHelper::buildRequest(const QString &apipart, const int &length, const QList<QPair<QString, QString> > &queryItems)
{
    QVariantMap account = config.getAccount();
    QString auth(account["uname"].toString() + ":" + account["pword"].toString());

    QUrl url = buildUrl(apipart);

    if (!queryItems.isEmpty()) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QUrlQuery query;
#endif
        for (int i = 0; i < queryItems.length(); ++i)
        {
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        query.addQueryItem(queryItems.at(i).first, queryItems.at(i).second);
#else
        url.addQueryItem(queryItems.at(i).first, queryItems.at(i).second);
#endif
        }

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        url.setQuery(query);
#endif
    }

#ifdef QT_DEBUG
    qDebug() << url.toString();
#endif

    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Authorization", "Basic " + auth.toUtf8().toBase64());

    if (length > 0) {
        request.setRawHeader("Content-Type", "application/json; charset=utf-8");
        request.setRawHeader("Content-Length", QByteArray::number(length));
    }

    return request;
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
