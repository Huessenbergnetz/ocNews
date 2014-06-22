#ifndef OCNAMINTERACTOR_H
#define OCNAMINTERACTOR_H

#include <QObject>
#include <QQmlEngine>
#include <QNetworkCookieJar>
#include <QNetworkAccessManager>

class OcNamInteractor : public QObject
{
    Q_OBJECT
public:
    explicit OcNamInteractor(QQmlEngine *qmlEngine, QObject *parent = 0);

signals:

public slots:
    Q_INVOKABLE void clearCookies();

private:
    QQmlEngine* m_qmlengine;

};

#endif // OCNAMINTERACTOR_H
