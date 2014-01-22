#ifndef OCHELPER_H
#define OCHELPER_H

#include <QObject>
#if defined(MEEGO_EDITION_HARMATTAN)
#include <qjson/parser.h>
#else
#include <QJsonDocument>
#include <QJsonObject>
#endif
#include "occonfiguration.h"

class OcHelper : public QObject
{
    Q_OBJECT
public:
    explicit OcHelper(QObject *parent = 0);
    QString buildUrl(QString apipart);
    QVariantMap jsonToQt(QByteArray json);

signals:

public slots:

private:
    OcConfiguration config;
    QSettings settings;

};

#endif // OCHELPER_H
