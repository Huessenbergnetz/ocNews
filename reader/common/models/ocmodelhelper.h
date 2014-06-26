#ifndef OCMODELHELPER_H
#define OCMODELHELPER_H

#include <QObject>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QRegularExpression>
#else
#include <QRegExp>
#endif
#include <QDateTime>
#include <QString>

class OcModelHelper : public QObject
{
    Q_OBJECT
public:
    explicit OcModelHelper(QObject *parent = 0);

    QString niceTime(const uint &t) const;
    QString prepareBody(const QString &b) const;

signals:

public slots:

};

#endif // OCMODELHELPER_H
