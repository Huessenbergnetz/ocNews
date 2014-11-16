//#include <QtNetwork>
#include "QsLog.h"
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
        QLOG_INFO() << "Can not request News app version: Device is in flight mode.";
        emit gotVersionError(tr("Device is in flight mode."));
    } else {
        QLOG_INFO() << "Start to request News app version.";

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

            QLOG_ERROR() << "Faild to request News app version: " << reply->errorString();
        }

        reply->deleteLater();

        notify.showNotification(errmsg, tr("Connection error"), OcNotifications::Error);

        emit gotVersionError(errmsg);

    } else {

        QVariantMap result = helper.jsonToQt(reply->readAll());

        if (result.contains("version"))
        {
            version = result["version"].toString();

            QLOG_INFO() << "Successfully requested News app version: " << version;

            emit gotVersion(version);

        } else if (result.contains("message")) {
            errmsg = result["message"].toString();

            QLOG_ERROR() << "Failed to request News app version: " << errmsg;

            emit gotVersionError(errmsg);
        } else {

            QLOG_ERROR() << "Failed to request News app version: Server reply was empty.";

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
        QLOG_WARN() << "Can not quit engine. Operation running.";
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();
    }
    QCoreApplication::quit();
}
