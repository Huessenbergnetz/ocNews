#ifndef OCLAUNCHER_H
#define OCLAUNCHER_H

#include <QObject>
#include <QProcess>

class OcLauncher : public QObject
{
    Q_OBJECT
public:
    explicit OcLauncher(QObject *parent = 0);
    Q_INVOKABLE void launch(const QString &executable, const QString &arguments);
    
signals:
    
public slots:

private:
    QProcess *m_process;
    
};

#endif // OCLAUNCHER_H
