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
        upgrade182AndOlder();
    } else if (oldVersion <= 182) {
        upgrade182AndOlder();
    }
}


bool OcUpgradeHelper::upgrade161AndOlder()
{
    qDebug() << "Performing internal upgrades for version 1.6.1 and lower.";

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

    return true;
}


bool OcUpgradeHelper::upgrade182AndOlder()
{
    qDebug() << "Performing internal upgrade for version 1.8.2 and lower.";

    QSqlQuery query;

    query.exec("DROP TRIGGER IF EXISTS folders_localUnread_move_feed");

    query.exec("CREATE TRIGGER IF NOT EXISTS folders_localUnread_move_feed AFTER UPDATE OF folderId ON feeds "
               "BEGIN "
               "UPDATE folders SET localUnreadCount = (SELECT SUM(localUnreadCount) FROM feeds WHERE folderId = new.folderId) WHERE id = new.folderId; "
               "UPDATE folders SET localUnreadCount = (SELECT SUM(localUnreadCount) FROM feeds WHERE folderId = old.folderId) WHERE id = old.folderId; "
               "END;");

    return true;
}
