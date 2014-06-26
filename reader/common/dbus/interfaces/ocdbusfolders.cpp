#include "ocdbusfolders.h"

OcDBusFolders::OcDBusFolders(QObject *parent) :
    QObject(parent)
{
    folders = new de::buschmann23::ocNewsEngine::Folders("de.buschmann23.ocNewsEngine", "/Folders", QDBusConnection::sessionBus(), this);
    connect(folders, SIGNAL(createdFolderError(QString)), this, SLOT(dbusCreatedFolderError(QString)));
    connect(folders, SIGNAL(createdFolderSuccess(QString,int)), this, SLOT(dbusCreatedFolderSuccess(QString,int)));
    connect(folders, SIGNAL(deletedFolderError(QString)), this, SLOT(dbusDeletedFolderError(QString)));
    connect(folders, SIGNAL(deletedFolderSuccess(int)), this, SLOT(dbusDeletedFolderSuccess(int)));
    connect(folders, SIGNAL(markedReadFolderError(QString)), this, SLOT(dbusMarkedReadFolderError(QString)));
    connect(folders, SIGNAL(markedReadFolderSuccess(int)), this, SLOT(dbusMarkedReadFolderSuccess(int)));
    connect(folders, SIGNAL(renamedFolderError(QString)), this, SLOT(dbusRenamedFolderError(QString)));
    connect(folders, SIGNAL(renamedFolderSuccess(QString, int)), this, SLOT(dbusRenamedFolderSuccess(QString, int)));
    connect(folders, SIGNAL(requestedFoldersError(QString)), this, SLOT(dbusRequestedFoldersError(QString)));
    connect(folders, SIGNAL(requestedFoldersSuccess(QList<int>, QList<int>, QList<int>)), this, SLOT(dbusRequestedFoldersSuccess(QList<int>, QList<int>, QList<int>)));
}

QVariantMap OcDBusFolders::getFolders()
{
    return folders->getFolders();
}

void OcDBusFolders::createFolder(const QString &name)
{
    folders->createFolder(name);
}

void OcDBusFolders::deleteFolder(const QString &id)
{
    folders->deleteFolder(id);
}

void OcDBusFolders::markFolderRead(const QString &id)
{
    folders->markFolderRead(id);
}

void OcDBusFolders::renameFolder(const QString &id, const QString &name)
{
    folders->renameFolder(id, name);
}

void OcDBusFolders::requestFolders()
{
    folders->requestFolders();
}

void OcDBusFolders::dbusCreatedFolderError(const QString &createresulterror)
{
    emit createdFolderError(createresulterror);
}

void OcDBusFolders::dbusCreatedFolderSuccess(const QString &foldername, const int &folderId)
{
    emit createdFolderSuccess(foldername, folderId);
}

void OcDBusFolders::dbusDeletedFolderError(const QString &deleteresulterror)
{
    emit deletedFolderError(deleteresulterror);
}

void OcDBusFolders::dbusDeletedFolderSuccess(const int &folderId)
{
    emit deletedFolderSuccess(folderId);
}

void OcDBusFolders::dbusMarkedReadFolderError(const QString &markedreaderror)
{
    emit markedReadFolderError(markedreaderror);
}

void OcDBusFolders::dbusMarkedReadFolderSuccess(const int &folderId)
{
    emit markedReadFolderSuccess(folderId);
}

void OcDBusFolders::dbusRenamedFolderError(const QString &renameresulterror)
{
    emit renamedFolderError(renameresulterror);
}

void OcDBusFolders::dbusRenamedFolderSuccess(const QString &newfoldername, const int &folderId)
{
    emit renamedFolderSuccess(newfoldername, folderId);
}

void OcDBusFolders::dbusRequestedFoldersError(const QString &requestresulterror)
{
    emit requestedFoldersError(requestresulterror);
}

void OcDBusFolders::dbusRequestedFoldersSuccess(const QList<int> &updated, const QList<int> &newFolders, const QList<int> &deleted)
{
    emit requestedFoldersSuccess(updated, newFolders, deleted);
}
