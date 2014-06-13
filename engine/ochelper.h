#ifndef OCHELPER_H
#define OCHELPER_H

#include <QObject>
#if defined(MEEGO_EDITION_HARMATTAN)
#include <qjson/parser.h>
#else
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#endif
#include "occonfiguration.h"
#include <QNetworkRequest>
#include <QUrl>

class OcHelper : public QObject
{
    Q_OBJECT
public:
    explicit OcHelper(QObject *parent = 0);
    QUrl buildUrl(QString apipart);
    QVariantMap jsonToQt(QByteArray json);
    QNetworkRequest buildRequest(const QString &apipart, const int &length = 0, const QList<QPair<QString, QString> > &queryItems = QList<QPair<QString, QString> >());

private:
    OcConfiguration config;
    QSettings settings;

};

#endif // OCHELPER_H
