#ifndef OCDBUSFOLDERS_H
#define OCDBUSFOLDERS_H

#include <QObject>

#include "ocfoldersif.h"

class OcDBusFolders : public QObject
{
    Q_OBJECT
public:
    explicit OcDBusFolders(QObject *parent = 0);

    Q_INVOKABLE QVariantMap getFolders();
    
signals:
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
    
public slots:
    void createFolder(const QString &name);
    void deleteFolder(const QString &id);
    void markFolderRead(const QString &id);
    void renameFolder(const QString &id, const QString &name);
    void requestFolders();

private slots:
    void dbusCreatedFolderError(const QString &createresulterror);
    void dbusCreatedFolderSuccess(const QString &foldername);
    void dbusDeletedFolderError(const QString &deleteresulterror);
    void dbusDeletedFolderSuccess();
    void dbusMarkedReadFolderError(const QString &markedreaderror);
    void dbusMarkedReadFolderSuccess(const int &folderId);
    void dbusRenamedFolderError(const QString &renameresulterror);
    void dbusRenamedFolderSuccess(const QString &newfoldername);
    void dbusRequestedFoldersError(const QString &requerstresulterror);
    void dbusRequestedFoldersSuccess(const QList<int> &updated, const QList<int> &newFolders, const QList<int> &deleted);

private:
    de::buschmann23::ocNewsEngine::Folders *folders;
    
};

#endif // OCDBUSFOLDERS_H
