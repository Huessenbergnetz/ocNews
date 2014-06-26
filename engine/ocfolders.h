#ifndef OCFOLDERS_H
#define OCFOLDERS_H

#include <QObject>
#include <QUrl>
#include "ocnetwork.h"
#include "ochelper.h"
#include "ocdbmanager.h"
#include "ocgeneric.h"
#include "ocnotifications.h"
#include "../common/globals.h"

class OcFolders : public QObject
{
    Q_OBJECT
public:
    explicit OcFolders(QObject *parent = 0);

public Q_SLOTS: // METHODS
    void createFolder(const QString &name);
    void deleteFolder(const QString &id);
    QVariantMap getFolders();
    void markFolderRead(const QString &id);
    void renameFolder(const QString &id, const QString &name);
    void requestFolders();

Q_SIGNALS: // SIGNALS
    void createdFolderError(const QString &createresulterror);
    void createdFolderSuccess(const QString &foldername);
    void deletedFolderError(const QString &deleteresulterror);
    void deletedFolderSuccess();
    void markedReadFolderError(const QString &markedreaderror);
    void markedReadFolderSuccess(const int &folderId);
    void renamedFolderError(const QString &renameresulterror);
    void renamedFolderSuccess(const QString &newfoldername);
    void requestedFoldersError(const QString &requerstresulterror);
    void requestedFoldersSuccess(const QList<int> &updated, const QList<int> &newFolders, const QList<int> &deleted);

signals:
    void deletedFolderCleanItems(const QList<int> &feedIds);

private slots:
    void foldersRequested();
    void folderCreated();
    void folderDeleted();
    void folderRenamed();
    void folderMarkedRead();

private:
    void foldersRequestedUpdateDb(const QVariantMap &foldersresult);
    void folderCreatedUpdateDb(const QVariantMap &createresult);
    void folderDeletedUpdateDb(const int &id);
    void folderRenamedUpdateDb(const int &id, const QString &name);
    void folderMarkedReadUpdateDb(const int &id);


    QNetworkReply *replyRequestFolders, *replyCreateFolder, *replyDeleteFolder, *replyRenameFolder, *replyMarkFolderRead;
    OcNotifications notify;
    OcHelper helper;
    OcDbManager database;
    OcNetwork network;
    OcConfiguration config;

    QString newFolderName;
};

#endif // OCFOLDERS_H
