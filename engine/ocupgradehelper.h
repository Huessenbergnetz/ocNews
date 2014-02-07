#ifndef OCUPGRADEHELPER_H
#define OCUPGRADEHELPER_H

#include <QObject>
#include <QSqlQuery>
#include <QStringList>
#include <QFileInfo>
#include <QVariant>

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
};

#endif // OCUPGRADEHELPER_H
