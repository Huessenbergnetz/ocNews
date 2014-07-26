//#include <QtNetwork>
#include <QDebug>
#include "ocgeneric.h"


/*!
 * \class OcGeneric
 * \brief The OcGeneric class provides some generic functions
 *
 * This class provides generic functions like version request, connectino init and
 * shutting down the daemon.
*/

OcGeneric::OcGeneric(QObject *parent) :
    QObject(parent)
{
}



/*!
 * \fn void OcGeneric::getVersion()
 * \brief Gets the version of the ownCloud News App
 *
 * This function requests the version of the ownCloud News App from the server.
 */

void OcGeneric::getVersion()
{
    if (network.isFlightMode())
    {
        emit gotVersionError(tr("Device is in flight mode."));
    } else {

        QNetworkRequest request = helper.buildRequest("version");
        reply = network.get(request);

        connect(reply,SIGNAL(finished()),this,SLOT(getVersionFinished()));
    }
}




/*!
 * \fn void OcGeneric::getVersionFinished()
 * \brief Handles the reply of getVersion()
 *
 * This internal class handles the network reply of the get request in getVersion().
 * If successful it emits the signal gotVersion() that contains the version string,
 * otherwise gotVersionError() is emitted.
 */

void OcGeneric::getVersionFinished()
{
    QString version;
    QString errmsg;

    if (reply->error()) {
        if (reply->error() == 5)
        {
            errmsg = tr("Request canceled, maybe wrong username or password");
        } else if (reply->error() == 3) {
            errmsg = tr("Remote host not found");
        } else if (reply->error() == 203) {
            errmsg = tr("Can not find ownCloud News, maybe wrong path or server");
        } else {
            errmsg = reply->errorString();
#ifdef QT_DEBUG
            qDebug() << reply->errorString();
#endif
        }

        reply->deleteLater();

        notify.showNotification(errmsg, tr("Connection error"), OcNotifications::Error);

        emit gotVersionError(errmsg);

    } else {

        QVariantMap result = helper.jsonToQt(reply->readAll());

        if (result.contains("version"))
        {
            version = result["version"].toString();
            emit gotVersion(version);

        } else if (result.contains("message")) {
            errmsg = result["message"].toString();
            emit gotVersionError(errmsg);
        } else {
            emit gotVersionError(tr("Server reply was empty."));
        }
    }
}



/*!
 * \fn void OcGeneric::quitEngine()
 * \brief Quits the engine/daemon
 *
 * This function quits the engine/daemon.
 */

void OcGeneric::quitEngine()
{
    while(inOperation) {
        qDebug() << "Operation running: " << inOperation;
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();
    }
    QCoreApplication::quit();
}
