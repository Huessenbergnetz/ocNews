#include "ocdbusfolders.h"

OcDBusFolders::OcDBusFolders(QObject *parent) :
    QObject(parent)
{
    folders = new de::buschmann23::ocNewsEngine::Folders("de.buschmann23.ocNewsEngine", "/Folders", QDBusConnection::sessionBus(), this);
    connect(folders, SIGNAL(createdFolderError(QString)), this, SLOT(dbusCreatedFolderError(QString)));
    connect(folders, SIGNAL(createdFolderSuccess(QString)), this, SLOT(dbusCreatedFolderSuccess(QString)));
    connect(folders, SIGNAL(deletedFolderError(QString)), this, SLOT(dbusDeletedFolderError(QString)));
    connect(folders, SIGNAL(deletedFolderSuccess()), this, SLOT(dbusDeletedFolderSuccess()));
    connect(folders, SIGNAL(markedReadFolderError(QString)), this, SLOT(dbusMarkedReadFolderError(QString)));
    connect(folders, SIGNAL(markedReadFolderSuccess()), this, SLOT(dbusMarkedReadFolderSuccess()));
    connect(folders, SIGNAL(renamedFolderError(QString)), this, SLOT(dbusRenamedFolderError(QString)));
    connect(folders, SIGNAL(renamedFolderSuccess(QString)), this, SLOT(dbusRenamedFolderSuccess(QString)));
    connect(folders, SIGNAL(requestedFoldersError(QString)), this, SLOT(dbusRequestedFoldersError(QString)));
    connect(folders, SIGNAL(requestedFoldersSuccess()), this, SLOT(dbusRequestedFoldersSuccess()));
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

void OcDBusFolders::dbusCreatedFolderSuccess(const QString &foldername)
{
    emit createdFolderSuccess(foldername);
}

void OcDBusFolders::dbusDeletedFolderError(const QString &deleteresulterror)
{
    emit deletedFolderError(deleteresulterror);
}

void OcDBusFolders::dbusDeletedFolderSuccess()
{
    emit deletedFolderSuccess();
}

void OcDBusFolders::dbusMarkedReadFolderError(const QString &markedreaderror)
{
    emit markedReadFolderError(markedreaderror);
}

void OcDBusFolders::dbusMarkedReadFolderSuccess()
{
    emit markedReadFolderSuccess();
}

void OcDBusFolders::dbusRenamedFolderError(const QString &renameresulterror)
{
    emit renamedFolderError(renameresulterror);
}

void OcDBusFolders::dbusRenamedFolderSuccess(const QString &newfoldername)
{
    emit renamedFolderSuccess(newfoldername);
}

void OcDBusFolders::dbusRequestedFoldersError(const QString &requestresulterror)
{
    emit requestedFoldersError(requestresulterror);
}

void OcDBusFolders::dbusRequestedFoldersSuccess()
{
    emit requestedFoldersSuccess();
}
