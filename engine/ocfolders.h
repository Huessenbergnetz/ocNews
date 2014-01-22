#ifndef OCFOLDERS_H
#define OCFOLDERS_H

#include <QObject>
#include <QUrl>
#include "ocnetwork.h"
#include "ochelper.h"
#include "ocdbmanager.h"
#include "ocgeneric.h"
#include "ocfeeds.h"

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

signals:
    void requestedFolders(QVariantMap foldersresult);
    void createdFolder(QVariantMap createresult);
    void deletedFolder(int id);
    void renamedFolder(int id, QString name);
    void markedReadFolder(int id);

Q_SIGNALS: // SIGNALS
    void createdFolderError(const QString &createresulterror);
    void createdFolderSuccess(const QString &foldername);
    void deletedFolderError(const QString &deleteresulterror);
    void deletedFolderSuccess();
    void markedReadFolderError(const QString &markedreaderror);
    void markedReadFolderSuccess();
    void renamedFolderError(const QString &renameresulterror);
    void renamedFolderSuccess(const QString &newfoldername);
    void requestedFoldersError(const QString &requerstresulterror);
    void requestedFoldersSuccess();

private slots:
    void foldersRequested();
    void foldersRequestedUpdateDb(QVariantMap foldersresult);
    void folderCreated();
    void folderCreatedUpdateDb(QVariantMap createresult);
    void folderDeleted();
    void folderDeletedUpdateDb(int id);
    void folderRenamed(QString name);
    void folderRenamedUpdateDb(int id, QString name);
    void folderMarkedRead();
    void folderMarkedReadUpdateDb(int id);

private:
    QUrl urlRequestFolders, urlCreateFolder, urlDeleteFolder, urlRenameFolder;
    QNetworkReply *replyRequestFolders, *replyCreateFolder, *replyDeleteFolder, *replyRenameFolder, *replyMarkFolderRead;
    OcHelper helper;
    OcDbManager database;
    OcFeeds feeds;
    OcNetwork network;
};

#endif // OCFOLDERS_H
