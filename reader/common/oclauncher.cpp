#include "oclauncher.h"
#include <QDBusInterface>

OcLauncher::OcLauncher(QObject *parent) :
    QObject(parent), m_process(new QProcess(this))
{
}

void OcLauncher::launch(const QString &executable, const QString &arguments)
{
    QStringList args = arguments.split(",");
    m_process->start(executable, args);
}


void OcLauncher::playAudio(const QString &playList)
{
    QStringList pl = playList.split(",");

    QDBusInterface audioInterface("com.nokia.maemo.meegotouch.MusicSuiteService", "/", "com.nokia.maemo.meegotouch.MusicSuiteInterface", QDBusConnection::sessionBus());
    audioInterface.call(QDBus::NoBlock, "play", pl);
}


void OcLauncher::playVideo(const QString &playList)
{
    QStringList pl = playList.split(",");

    QDBusInterface videoInterface("com.nokia.maemo.meegotouch.VideoSuiteService", "/", "com.nokia.maemo.meegotouch.VideoSuiteInterface", QDBusConnection::sessionBus());
    videoInterface.call(QDBus::NoBlock, "play", pl);
}
