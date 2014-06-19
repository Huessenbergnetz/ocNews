#include <QDebug>
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
    QObject(parent)
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

    config["maxitems"] = settings.value("storage/maxitems", "100");
    config["viewmode"] = settings.value("display/viewmode", 0).toInt();
    config["orderby"] = settings.value("display/orderby", "id");
    config["themecolor"] = settings.value("display/themecolor", "white");
    config["handleimgs"] = settings.value("display/handleimgs", 0).toInt();
    config["handleread"] = settings.value("display/handleread", 0).toInt();
    config["sortasc"] = settings.value("display/sortasc", false).toBool();
    config["fontsize"] = settings.value("display/fontsize", DEFAULT_FONT_SIZE).toInt();
    config["hidereadfeeds"] = settings.value("display/hidereadfeeds", false).toBool();
    config["showExcerpts"] = settings.value("display/excerpts", false).toBool();
    config["updatebehavior"] = settings.value("update/behavior", "0");
    config["updateinterval"] = settings.value("update/interval", "3600");
    config["eventfeeds"] = settings.value("event/feeds", "");
    config["quitengine"] = settings.value("engine/quitonclose", false).toBool();
    config["notifyFeedsFolders"] = settings.value("notifications/feedsFolders", false).toBool();
    config["notifyNewItems"] = settings.value("notifications/newItems", false).toBool();
#if !defined(MEEGO_EDITION_HARMATTAN)
    config["enabled"] = settings.value("account/enabled", true).toBool();
    config["uname"] = settings.value("account/user", "").toString();
    config["pword"] = settings.value("account/password", "").toString();
    config["server"] = settings.value("account/server", "").toString();
    config["usessl"] = settings.value("account/usessl", true).toBool();
    config["ignoresslerrors"] = settings.value("account/ignoresslerrors", false).toBool();
#else
    config["textformat"] = settings.value("display/textformat", "rich");
#endif

    emit gotConfig(config);

    return config;
}



/*!
 * \fn void OcConfiguration::saveConfig(const QVariantMap &config)
 * \brief Saves the default configuration options
 *
 * This functions saves the main configuration options that it get in
 * a QVariantMap. If used, you have to set every mapped item.
 *
 * \param config QVariantMap that contains the main configuration
 */

void OcConfiguration::saveConfig(const QVariantMap &config)
{
    settings.setValue("storage/maxitems", config["maxitems"]);
    settings.setValue("display/viewmode", config["viewmode"]);
    settings.setValue("display/orderby", config["orderby"]);
    settings.setValue("display/themecolor", config["themecolor"]);
    settings.setValue("display/handleimgs", config["handleimgs"]);
    settings.setValue("display/handleread", config["handleread"]);
    settings.setValue("display/sortasc", config["sortasc"]);
    settings.setValue("display/fontsize", config["fontsize"]);
    settings.setValue("display/hidereadfeeds", config["hidereadfeeds"]);
    settings.setValue("display/excerpts", config["showExcerpts"]);
    settings.setValue("update/behavior", config["updatebehavior"]);
    settings.setValue("update/interval", config["updateinterval"]);
    settings.setValue("event/feeds", config["eventfeeds"]);
    settings.setValue("engine/quitonclose", config["quitengine"]);
    settings.setValue("notifications/feedsFolders", config["notifyFeedsFolders"]);
    settings.setValue("notifications/newItems", config["notifyNewItems"]);
#if !defined(MEEGO_EDITION_HARMATTAN)
    settings.setValue("account/enabled", config["enabled"]);
    settings.setValue("account/user", config["uname"]);
    settings.setValue("account/password", config["pword"]);
    settings.setValue("account/server", config["server"]);
    settings.setValue("account/usessl", config["usessl"]);
    settings.setValue("account/ignoresslerrors", config["ignoresslerrors"]);
#else
    settings.setValue("display/textformat", config["textformat"]);
#endif
    settings.sync();

    emit savedConfig();

#ifdef QT_DEBUG
        qDebug() << "Saved configuration";
#endif
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
    qvResult = settings.value(entry, defaultValue.variant());
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
#ifdef QT_DEBUG
        qDebug() << "Set Setting: " << entry << " : " << setvalue;
#endif
    settings.setValue(entry, setvalue);

    if (entry == "account/id")
        emit changedConfig();
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
    settings.remove("server");
    settings.remove("display");
    settings.remove("storage/maxitems");
    settings.remove("update");
    settings.remove("event");
#if !defined(MEEGO_EDITION_HARMATTAN)
    settings.remove("account");
#endif

    getConfig();
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
    QSqlQuery query;
    query.exec("DELETE FROM folders");
    query.exec("DELETE FROM feeds");
    query.exec("DELETE FROM items");
    query.exec("DELETE FROM queue");
    settings.remove("storage/lastFullUpdate");

    getStatistics();
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
        QSqlQuery query;

        query.exec(QString("SELECT COUNT(id) FROM items WHERE unread = ").append(SQL_TRUE));

        if(query.next())
            qvResult = query.value(0);
    } else if (stat == 1) {
        qvResult = settings.value("storage/lastFullUpdate", 0) == 0 ? 0 :
                   QDateTime::fromTime_t(settings.value("storage/lastFullUpdate",0).toInt()).toLocalTime().toMSecsSinceEpoch();
    }

    result.setVariant(qvResult);

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
    QVariantMap stats;
    QSqlQuery query;

    query.exec(QString("SELECT (SELECT COUNT(id) FROM items) AS items, "
               "(SELECT COUNT(id) FROM items WHERE unread = %1) AS unreadItems, "
               "(SELECT COUNT(id) FROM folders) AS folders, "
               "(SELECT COUNT(id) FROM feeds) AS feeds").arg(SQL_TRUE));

    if(query.next())
    {
        stats["itemCount"] = query.value(0);
        stats["unreadCount"] = query.value(1);
        stats["folderCount"] = query.value(2);
        stats["feedCount"] = query.value(3);
        stats["lastFullUpdate"] = settings.value("storage/lastFullUpdate", 0) == 0 ? 0 :
                                  QDateTime::fromTime_t(settings.value("storage/lastFullUpdate", 0).toInt()).toLocalTime().toMSecsSinceEpoch();
    }

    emit gotStatistics(stats);

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

#if defined(MEEGO_EDITION_HARMATTAN)
    // set credential for ssl domain
    int credSuc = aegis_certman_set_credentials("buschtrommel-ocnews::CertOCNewsSSL");
    if (credSuc != 0) qDebug() << "set credential error: " << credSuc;

    // open ssl domain
    domain_handle ownDomain;
    int openCheck = aegis_certman_open_domain("ssl-ocnews", AEGIS_CERTMAN_DOMAIN_PRIVATE, &ownDomain);
    if (openCheck != 0) qDebug() << "Error Opening Domain: " << openCheck;

    aegis_certman_iterate_certs(ownDomain, &collectCertsForDeletion, NULL);

    if (!certsToDelete.isEmpty())
    {
        for (int i = 0; i < certsToDelete.size(); ++i)
        {
            aegis_key_id crtKey;
            aegis_certman_str_to_key_id(certsToDelete.at(i).toLocal8Bit().data(),crtKey);
            int rmCertCheck = aegis_certman_rm_cert(ownDomain, crtKey);
            if (rmCertCheck != 0)
                qDebug() << "Error removing Certificate from private domain.";
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

    QString accountid = settings.value("account/id", "").toString();

    if (accountid != "") {

        accMan = new Accounts::Manager();

        Accounts::AccountId id;

        id = accountid.toInt();

        account = accMan->account(id);

        if (account != NULL)
            configStatus = true;
    }

#else

    QString uname = settings.value("account/user", "").toString();
    QString pword = settings.value("account/password", "").toString();
    QString server = settings.value("account/server", "").toString();

    if (uname != "" && pword != "" && server != "")
        configStatus = true;

#endif

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
    id = settings.value("account/id", "").toInt();

    account = accMan->account(id);

    if (account != NULL && account->enabled())
        accountState = true;

#else

    bool enabled = settings.value("account/enabled", false).toBool();
    if (enabled)
        accountState = true;

#endif

#ifdef QT_DEBUG
    qDebug() << "Account State: " << accountState;
#endif

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
    QVariantMap result;
    if (isConfigSet())
    {

#if defined(MEEGO_EDITION_HARMATTAN)
        accMan = new Accounts::Manager();

        Accounts::AccountId id;
        id = settings.value("account/id", "").toInt();

        account = accMan->account(id);

#ifdef QT_DEBUG
        qDebug() << "Acc Manager: " << accMan;
        qDebug() << "Account ID: " << id;
        qDebug() << "Account: " << account;
        qDebug() << "Last account error: " << accMan->lastError().message();
#endif
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

        bool enabled = settings.value("account/enabled", false).toBool();
        QString uname = settings.value("account/user", "").toString();
        QString pword = settings.value("account/password", "").toString();
        QString server = settings.value("account/server", "").toString();
        bool usessl = settings.value("account/usessl", true).toBool();
        bool ignoresslerror = settings.value("account/ignoresslerrors", false).toBool();

        if (!isConfigSet())
        {
            result["state"] = 2;
        } else if (isConfigSet() && enabled == false) {
            result["state"] = 3;
        } else if (isConfigSet() && enabled == true) {
            result["state"] = 0;
            result["uname"] = settings.value("account/user", "").toString();
            result["pword"] = settings.value("account/password", "").toString();
            result["server"] = settings.value("account/server", "").toString();
            result["usessl"] = settings.value("account/usessl", true).toBool();
            result["ignoresslerror"] = settings.value("account/ignoresslerrors", false).toBool();
        }

#endif

    } else {
        result["state"] = 1;
    }

    return result;
}
