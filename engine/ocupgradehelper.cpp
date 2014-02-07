#include <QDebug>
#include "ocupgradehelper.h"

OcUpgradeHelper::OcUpgradeHelper(QObject *parent) :
    QObject(parent)
{
}


void OcUpgradeHelper::init(int oldVersion, int currentVersion)
{
    if (oldVersion <= 161)
    {
        upgrade161AndOlder();
    }
}


bool OcUpgradeHelper::upgrade161AndOlder()
{
    qDebug() << "Performing internal upgrades vor version 1.6.1 and lower.";

    QSqlQuery query;

    QStringList iconSourceList;
    query.exec("SELECT iconSource FROM feeds");
    while(query.next())
        iconSourceList << query.value(0).toString();

    for (int i = 0; i < iconSourceList.size(); ++i)
    {
        if (iconSourceList.at(i) != "")
        {
            QFileInfo fileInfo(iconSourceList.at(i));

            query.prepare("UPDATE feeds SET iconSource = :newSource WHERE iconSource = :oldSource");
            query.bindValue(":oldSource", iconSourceList.at(i));
            query.bindValue(":newSource", fileInfo.fileName());
            query.exec();
        }

    }
}
