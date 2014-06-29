#ifndef OCUPGRADEHELPER_H
#define OCUPGRADEHELPER_H

#include <QObject>
#include <QSqlQuery>
#include <QStringList>
#include <QFileInfo>
#include <QVariant>
#include <QSettings>
#include "../common/globals.h"

class OcUpgradeHelper : public QObject
{
    Q_OBJECT
public:
    explicit OcUpgradeHelper(QObject *parent = 0);
    void init(int oldVersion, int currentVersion);

signals:

public slots:


private:
    bool upgrade161AndOlder();
    bool upgrade182AndOlder();

    QSettings settings;
};

#endif // OCUPGRADEHELPER_H
