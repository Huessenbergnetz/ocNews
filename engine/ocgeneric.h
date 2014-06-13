#ifndef OCGENERIC_H
#define OCGENERIC_H

#include <QObject>
#include <QUrl>
#include <QEventLoop>
#include <QCoreApplication>
#include <QTimer>
#include "ochelper.h"
#include "ocnetwork.h"
#include "occonfiguration.h"
#include "../common/globals.h"

class OcGeneric : public QObject
{
    Q_OBJECT
public:
    explicit OcGeneric(QObject *parent = 0);

public Q_SLOTS: // METHODS
    void getVersion();
    void initConnection();
    void quitEngine();

Q_SIGNALS: // SIGNALS
    void gotVersion(const QString &version);
    void gotVersionError(const QString &error);
    void initError(const QString &errorMessage);
    void initErrorFlightMode();
    void initSuccess();

private slots:
    void getVersionFinished();
    void initFinished();

private:
    QUrl url;
    OcNetwork network;
    QNetworkReply *reply;
    OcHelper helper;
    OcConfiguration configuration;

};

#endif // OCGENERIC_H
