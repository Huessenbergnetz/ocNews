#ifndef OCSHAREUI_H
#define OCSHAREUI_H

#include <QObject>
#include <MDataUri/MDataUri>
#include <maemo-meegotouch-interfaces/shareuiinterface.h>

class OcShareUi : public QObject
{
    Q_OBJECT
public:
    explicit OcShareUi(QObject *parent = 0);
    
signals:
    
public slots:
    void share(const QString &link, const QString &title, const QString &description);

private:


};

#endif // OCSHAREUI_H
