#include "QsLog.h"
#include "occonfiguration.h"

#if defined(MEEGO_EDITION_HARMATTAN)
QStringList certsToDelete;
int collectCertsForDeletion(int ordering, X509* cert, void* ctx)
{
    aegis_key_id crtKey;
    aegis_certman_get_key_id(cert, crtKey);

    char buffer[AEGIS_KEY_ID_STR_LEN];

    aegis_certman_key_id_to_str(crtKey, buffer, AEGIS_KEY_ID_STR_LEN);

    certsToDelete.append(buffer);

    return 0;
}
#endif



/*!
 * \class OcConfiguration
 * \brief The OcConfiguration class provides an abstraction to the configuration.
 *
 * This class implements functions to set and retrieve configuratin options. It is
 * an abstraction to system specific configuration systems for options, account
 * settings and maintenance operations.
*/

OcConfiguration::OcConfiguration(QObject *parent) :
    QSettings(parent)
{
}





/*!
 * \fn QVariantMap OcConfiguration::getConfig()
 * \brief Retrieves default configuration options
 *
 * This function retrieves the main configuration options as a QVariantMap
 * and emits the same result in a signal.
 *
 * \return main configuration options
 */


QVariantMap OcConfiguration::getConfig()
{
    QVariantMap config;

    config["maxitems"] = value("storage/maxitems", 100).toInt();
    config["viewmode"] = value("display/viewmode", 0).toInt();
    config["orderby"] = value("display/orderby", "id").toString();
    config["handleimgs"] = value("display/handleimgs", 0).toInt();
    config["handleread"] = value("display/handleread", 0).toInt();
    config["sortasc"] = value("display/sortasc", false).toBool();
    config["fontsize"] = value("display/fontsize", DEFAULT_FONT_SIZE).toInt();
    config["hidereadfeeds"] = value("display/hidereadfeeds", false).toBool();
    config["showExcerpts"] = value("display/excerpts", false).toBool();
    config["showPicturesInList"] = value("display/picturesInList", false).toBool();
    config["privacyShown"] = value("display/privacypolicy", false).toBool();
    config["displayversion"] = value("display/version", 0).toInt();
    config["privateBrowsing"] = value("privacy/privateBrowsing", false).toBool();
    config["enableCookies"] = value("privacy/enableCookies", true).toBool();
    config["updatebehavior"] = value("update/behavior", 0).toInt();
    config["updateinterval"] = value("update/interval", 3600).toInt();
    config["eventfeeds"] = value("event/feeds", "").toString();
    config["quitengine"] = value("engine/quitonclose", false).toBool();
    config["notifyFeedsFolders"] = value("notifications/feedsFolders", false).toBool();
    config["notifyNewItems"] = value("notifications/newItems", false).toBool();
#if !defined(MEEGO_EDITION_HARMATTAN)
    config["enabled"] = value("account/enabled", true).toBool();
    config["uname"] = value("account/user", "").toString();
    config["pword"] = value("account/password", "").toString();
    config["server"] = value("account/server", "").toString();
    config["usessl"] = value("account/usessl", true).toBool();
    config["ignoresslerrors"] = value("account/ignoresslerrors", false).toBool();
#else
    config["userichtext"] = value("display/richText", true).toBool();
    config["themeinverted"] = value("display/themeInverted", false).toBool();
#endif

    emit gotConfig(config);

    return config;
}



/*!
 * \fn QDBusVariant OcConfiguration::getSetting(const QString &entry, const QDBusVariant &defaultValue)
 * \brief Gets an entry from the settings
 *
 * This function gets a single entry from the settings system.
 *
 * \param entry QString the entry to get
 * \param defaultValue QDBusVariant the default value if entry is not set
 * \return the setting value or the default value in a QDBusVariant
 */

QDBusVariant OcConfiguration::getSetting(const QString &entry, const QDBusVariant &defaultValue)
{
    QDBusVariant result;
    QVariant qvResult;
    qvResult = value(entry, defaultValue.variant());
    result.setVariant(qvResult);

    return result;
}



/*!
 * \fn void OcConfiguration::setSetting(const QString &entry, const QDBusVariant &value)
 * \brief Sets an entry in the settings
 *
 * This funtion sets a single entry in the settings system.
 *
 * \param entry QString that specifies the entry to set
 * \param value QDBusVariant that specifies the value to set
 */

void OcConfiguration::setSetting(const QString &entry, const QDBusVariant &value)
{
    QVariant setvalue = value.variant();

    QLOG_DEBUG() << "Set Setting: " << entry << " : " << setvalue;

    setValue(entry, setvalue);

    if (entry == "account/id")
        emit changedConfig();

    if (entry == "update/interval")
        emit updateIntervalChanged(setvalue.toInt());

    if (entry == "update/behavior")
        emit updateBehaviorChanged(setvalue.toInt());
}




/*!
 * \fn void OcConfiguration::resetConfig()
 * \brief Resets the main configuration
 *
 * This function resets the main configuration options,
 * respectively deletes them. After the deletion it calls
 * getConfig() to emit the gotConfig() signal.
 */

void OcConfiguration::resetConfig()
{
    QLOG_INFO() << "Resetting configuration";
   remove("server");
   remove("display");
   remove("storage/maxitems");
   remove("update");
   remove("event");
#if !defined(MEEGO_EDITION_HARMATTAN)
   remove("account");
#endif

   emit configReset();

}



/*!
 * \fn void OcConfiguration::cleanDatabase()
 * \brief Cleans the database
 *
 * This function deletes all entries in the database, calls getStatistics()
 * to emit the gotStatistics() signal and emits it's own signal cleanedDatabse(). *
 */

void OcConfiguration::cleanDatabase()
{
    QLOG_INFO() << "Cleaning database";
    QSqlQuery query;
    query.exec("DELETE FROM folders");
    query.exec("DELETE FROM feeds");
    query.exec("DELETE FROM items");
    query.exec("DELETE FROM queue");
    query.exec("DELETE FROM images");
    remove("storage/lastFullUpdate");

    emit cleanedDatabase();
}





/*!
 * \fn QDBusVariant OcConfiguration::getStat(const int stat)
 * \brief Calculates a single statistic
 *
 * This function calculates a single statistic out of the database
 *
 * \param stat      int that indicates the value to retrieve, 0 for unread items, 1 for last full update
 * \return the statistical value as QDBusVariant
 */

QDBusVariant OcConfiguration::getStat(const int stat)
{
    QDBusVariant result;
    QVariant qvResult = 0;

    if (stat == 0) {
        QLOG_DEBUG() << "Getting all unread items statistic:";
        QSqlQuery query;

        query.exec(QString("SELECT COUNT(id) FROM items WHERE unread = ").append(SQL_TRUE));

        if(query.next())
            qvResult = query.value(0);
    } else if (stat == 1) {
        QLOG_DEBUG() << "Getting last full update statistic:";
        qvResult = value("storage/lastFullUpdate", 0) == 0 ? 0 :
                   QDateTime::fromTime_t(value("storage/lastFullUpdate",0).toInt()).toLocalTime().toMSecsSinceEpoch();
    }

    result.setVariant(qvResult);

    QLOG_DEBUG() << "Statistic result: " << qvResult;

    return result;

}




/*!
 * \fn QVariantMap OcConfiguration::getStatistics()
 * \brief Retrieves statistic information about the database.
 *
 * This function retrieves statistical information about the
 * database. Count of items, unread items, folders and feeds
 * as well as the time of the last full update. Beside retreiving
 * the statistics it emits the same result in the signal gotStatistics().
 *
 * \return the statistics in a QVariantMap
 */

QVariantMap OcConfiguration::getStatistics()
{
    QLOG_DEBUG() << "Getting statistics:";

    QVariantMap stats;
    QSqlQuery query;

    query.exec(QString("SELECT (SELECT COUNT(id) FROM items) AS items, "
               "(SELECT COUNT(id) FROM items WHERE unread = %1) AS unreadItems, "
               "(SELECT COUNT(id) FROM folders) AS folders, "
               "(SELECT COUNT(id) FROM feeds) AS feeds").arg(SQL_TRUE));

    if(query.next())
    {
        stats["itemCount"] = query.value(0).toInt();
        stats["unreadCount"] = query.value(1).toInt();
        stats["folderCount"] = query.value(2).toInt();
        stats["feedCount"] = query.value(3).toInt();
        stats["lastFullUpdate"] = value("storage/lastFullUpdate", 0) == 0 ? 0 :
                                  QDateTime::fromTime_t(value("storage/lastFullUpdate", 0).toInt()).toLocalTime().toMSecsSinceEpoch();
    }

    emit gotStatistics(stats);

    QLOG_TRACE() << stats;

    return stats;
}





/*!
 * \fn void OcConfiguration::cleanCertificates()
 * \brief Cleans the accepted certificates
 *
 * This funtion cleans the already accepted certificates
 * and emits the signal cleanedCertificates() after deletion.
 */

void OcConfiguration::cleanCertificates()
{
    QLOG_INFO() << "Cleaning certificates";

#if defined(MEEGO_EDITION_HARMATTAN)
    // set credential for ssl domain
    int credSuc = aegis_certman_set_credentials("buschtrommel-ocnews::CertOCNewsSSL");
    if (credSuc != 0) QLOG_ERROR() << "set credential error: " << credSuc;

    // open ssl domain
    domain_handle ownDomain;
    int openCheck = aegis_certman_open_domain("ssl-ocnews", AEGIS_CERTMAN_DOMAIN_PRIVATE, &ownDomain);
    if (openCheck != 0) QLOG_ERROR() << "Error Opening Domain: " << openCheck;

    aegis_certman_iterate_certs(ownDomain, &collectCertsForDeletion, NULL);

    if (!certsToDelete.isEmpty())
    {
        for (int i = 0; i < certsToDelete.size(); ++i)
        {
            aegis_key_id crtKey;
            aegis_certman_str_to_key_id(certsToDelete.at(i).toLocal8Bit().data(),crtKey);
            int rmCertCheck = aegis_certman_rm_cert(ownDomain, crtKey);
            if (rmCertCheck != 0)
                QLOG_ERROR() << "Error removing Certificate from private domain.";
        }
    }
#endif

    emit cleanedCertificates();
}




/*!
 * \fn bool OcConfiguration::isConfigSet()
 * \brief Checks if an account is set.
 *
 * This function checks if an account is set and
 * returns true if an account is set.
 *
 * \return true if account is set
 */

bool OcConfiguration::isConfigSet()
{
    bool configStatus = false;

#if defined(MEEGO_EDITION_HARMATTAN)

    QString accountid = value("account/id", "").toString();

    if (accountid != "") {

        accMan = new Accounts::Manager();

        Accounts::AccountId id;

        id = accountid.toInt();

        account = accMan->account(id);

        if (account != NULL)
            configStatus = true;
    }

#else

    QString uname = value("account/user", "").toString();
    QString pword = value("account/password", "").toString();
    QString server = value("account/server", "").toString();

    if (uname != "" && pword != "" && server != "")
        configStatus = true;

#endif

    QLOG_DEBUG() << "Config status: " << configStatus;

    return configStatus;
}



/*!
 * \fn bool OcConfiguration::isAccountEnabled()
 * \brief Checks if the account is enabled
 *
 * This function checks if the account is enabled.
 *
 * \return true if account is enabled
 */

bool OcConfiguration::isAccountEnabled()
{

    bool accountState = false;

#if defined(MEEGO_EDITION_HARMATTAN)

    accMan = new Accounts::Manager();

    Accounts::AccountId id;
    id = value("account/id", "").toInt();

    account = accMan->account(id);

    if (account != NULL && account->enabled())
        accountState = true;

#else

    if (value("account/enabled", true).toBool())
        accountState = true;

#endif

    QLOG_DEBUG() << "Account State: " << accountState;

    return accountState;
}




/*!
 * \fn QVariantMap OcConfiguration::getAccount()
 * \brief Retrieves the account information
 *
 * This function retrieves the account data/credentials in a QVariantMap.
 *
 * \return account data in a QVariantMap
 */

QVariantMap OcConfiguration::getAccount()
{
    QLOG_DEBUG() << "Getting account data";

    QVariantMap result;
    if (isConfigSet())
    {

#if defined(MEEGO_EDITION_HARMATTAN)
        accMan = new Accounts::Manager();

        Accounts::AccountId id;
        id = value("account/id", "").toInt();

        account = accMan->account(id);

        QLOG_DEBUG() << "Acc Manager: " << accMan;
        QLOG_DEBUG() << "Account ID: " << id;
        QLOG_DEBUG() << "Account: " << account;
        QLOG_DEBUG() << "Last account error: " << accMan->lastError().message();

        if (account == NULL)
        {
            result["state"] = 2;
        } else if (account != NULL && account->enabled() == false) {
            result["state"] = 3;
        } else if (account != NULL && account->enabled()) {
            result["state"] = 0;
            result["uname"] = account->valueAsString("username");
            result["pword"] = account->valueAsString("password");
            result["server"] = account->valueAsString("server");
            result["usessl"] = account->valueAsBool("usessl");
            result["ignoresslerror"] = account->valueAsBool("ignoresslerrors");
        }
#else

        bool enabled = value("account/enabled", true).toBool();
        QString uname = value("account/user", "").toString();
        QString pword = value("account/password", "").toString();
        QString server = value("account/server", "").toString();
        bool usessl = value("account/usessl", true).toBool();
        bool ignoresslerror = value("account/ignoresslerrors", false).toBool();

        if (!isConfigSet())
        {
            result["state"] = 2;
        } else if (isConfigSet() && enabled == false) {
            result["state"] = 3;
        } else if (isConfigSet() && enabled == true) {
            result["state"] = 0;
            result["uname"] = uname;
            result["pword"] = pword;
            result["server"] = server;
            result["usessl"] = usessl;
            result["ignoresslerror"] = ignoresslerror;
        }

#endif

    } else {
        result["state"] = 1;
    }

#ifdef QT_DEBUG
    qDebug() << result;
#endif

    return result;
}
