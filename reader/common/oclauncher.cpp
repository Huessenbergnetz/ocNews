#include "oclauncher.h"

OcLauncher::OcLauncher(QObject *parent) :
    QObject(parent), m_process(new QProcess(this))
{
}

void OcLauncher::launch(const QString &executable, const QString &arguments)
{
    QStringList args = arguments.split(",");
    m_process->start(executable, args);
}
