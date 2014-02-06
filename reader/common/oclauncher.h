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
    Q_INVOKABLE void playAudio(const QString &playList);
    Q_INVOKABLE void playVideo(const QString &playList);
    
signals:
    
public slots:

private:
    QProcess *m_process;
    
};

#endif // OCLAUNCHER_H
