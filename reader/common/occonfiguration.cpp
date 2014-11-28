#include "occonfiguration.h"
#include <QDebug>

OcConfiguration::OcConfiguration(QObject *parent) :
    QObject(parent)
{
    conf = new de::buschmann23::ocNewsEngine::Configuration("de.buschmann23.ocNewsEngine", "/Configuration", QDBusConnection::sessionBus(), this);

    QVariantMap config = conf->getConfig();
    m_privateBrowsing = config["privateBrowsing"].toBool();
    m_enableCookies = config["enableCookies"].toBool();
    m_maxItems = config["maxitems"].toInt();
    m_viewMode = config["viewmode"].toInt();
    m_orderBy = config["orderby"].toString();
    m_handleImgs = config["handleimgs"].toInt();
    m_handleRead = config["handleread"].toInt();
    m_sortAsc = config["sortasc"].toBool();
    m_fontSize = config["fontsize"].toInt();
    m_hideReadFeeds = config["hidereadfeeds"].toBool();
    m_showExcerpts = config["showExcerpts"].toBool();
    m_showPicturesInList = config["showPicturesInList"].toBool();
    m_updateBehavior = config["updatebehavior"].toInt();
    m_updateInterval = config["updateinterval"].toInt();
    m_eventFeeds = config["eventfeeds"].toString();
    m_quitEngine = config["quitengine"].toBool();
    m_notifyFeedsFolders = config["notifyFeedsFolders"].toBool();
    m_notifyNewItems = config["notifyNewItems"].toBool();
    m_isValid = conf->isConfigSet();
    m_privacyShown = config["privacyShown"].toBool();
    m_displayedVersion = config["displayversion"].toInt();
    m_createLogFile = config["createLogFile"].toBool();
    m_displayLanguage = config["displayLanguage"].toString();
    m_directLinkOpening = config["directLinkOpening"].toBool();
#if !defined(MEEGO_EDITION_HARMATTAN)
    m_accountEnabled = config["enabled"].toBool();
    m_accountUser = config["uname"].toString();
    m_accountPassword = config["pword"].toString();
    m_accountServer = config["server"].toString();
    m_accountUseSSL = config["usessl"].toBool();
    m_accountIgnoreSSLErrors = config["ignoresslerrors"].toBool();
#else
    m_useRichText = config["userichtext"].toBool();
    m_themeInverted = config["themeinverted"].toBool();
#endif
}


int OcConfiguration::displayedVersion() const { return m_displayedVersion; }

void OcConfiguration::setDisplayedVersion(const int &nDisplayedVersion)
{
    if (nDisplayedVersion != m_displayedVersion) {
        m_displayedVersion = nDisplayedVersion;
        conf->setSetting("display/version", QDBusVariant(displayedVersion()));
        emit displayedVersionChanged(displayedVersion());
    }
}


bool OcConfiguration::privateBrowsing() const { return m_privateBrowsing; }

void OcConfiguration::setPrivateBrowsing(const bool &nPrivateBrowsing)
{
    if (nPrivateBrowsing != m_privateBrowsing) {
        m_privateBrowsing = nPrivateBrowsing;
        conf->setSetting("privacy/privateBrowsing", QDBusVariant(privateBrowsing()));
        emit privateBrowsingChanged(privateBrowsing());
    }
}


bool OcConfiguration::enableCookies() const { return m_enableCookies; }

void OcConfiguration::setEnableCookies(const bool &nEnableCookies)
{
    if (nEnableCookies != m_enableCookies) {
        m_enableCookies = nEnableCookies;
        conf->setSetting("privacy/enableCookies", QDBusVariant(enableCookies()));
        emit enableCookiesChanged(enableCookies());
    }
}


int OcConfiguration::maxItems() const { return m_maxItems; }

void OcConfiguration::setMaxItems(const int &nMaxItems)
{
    if (nMaxItems != m_maxItems) {
        m_maxItems = nMaxItems;
        conf->setSetting("storage/maxitems", QDBusVariant(maxItems()));
        emit maxItemsChanged(maxItems());
    }
}


int OcConfiguration::viewMode() const { return m_viewMode; }

void OcConfiguration::setViewMode(const int &nViewMode)
{
    if (nViewMode != m_viewMode) {
        m_viewMode = nViewMode;
        conf->setSetting("display/viewmode", QDBusVariant(viewMode()));
        emit viewModeChanged(viewMode());
    }
}


QString OcConfiguration::orderBy() const { return m_orderBy; }

void OcConfiguration::setOrderBy(const QString &nOrderBy)
{
    if (nOrderBy != m_orderBy) {
        m_orderBy = nOrderBy;
        conf->setSetting("display/orderby", QDBusVariant(orderBy()));
        emit orderByChanged(orderBy());
    }
}


int OcConfiguration::handleImgs() const { return m_handleImgs; }

void OcConfiguration::setHandleImgs(const int &nHandleImgs)
{
    if (nHandleImgs != m_handleImgs) {
        m_handleImgs = nHandleImgs;
        conf->setSetting("display/handleimgs", QDBusVariant(handleImgs()));
        emit handleImgsChanged(handleImgs());
    }
}


int OcConfiguration::handleRead() const { return m_handleRead; }

void OcConfiguration::setHandleRead(const int &nHandleRead)
{
    if (nHandleRead != m_handleRead) {
        m_handleRead = nHandleRead;
        conf->setSetting("display/handleread", QDBusVariant(handleRead()));
        emit handleReadChanged(handleRead());
    }
}


bool OcConfiguration::sortAsc() const { return m_sortAsc; }

void OcConfiguration::setSortAsc(const bool &nSortAsc)
{
    if (nSortAsc != m_sortAsc) {
        m_sortAsc = nSortAsc;
        conf->setSetting("display/sortasc", QDBusVariant(sortAsc()));
        emit sortAscChanged(sortAsc());
    }
}



int OcConfiguration::fontSize() const { return m_fontSize; }

void OcConfiguration::setFontSize(const int &nFontSIze)
{
    if (nFontSIze != m_fontSize) {
        m_fontSize = nFontSIze;
        conf->setSetting("display/fontsize", QDBusVariant(fontSize()));
        emit fontSizeChanged(fontSize());
    }
}


bool OcConfiguration::hideReadFeeds() const { return m_hideReadFeeds; }

void OcConfiguration::setHideReadFeeds(const bool &nHideReadFeeds)
{
    if (nHideReadFeeds != m_hideReadFeeds) {
        m_hideReadFeeds = nHideReadFeeds;
        conf->setSetting("display/hidereadfeeds", QDBusVariant(hideReadFeeds()));
        emit hideReadFeedsChanged(hideReadFeeds());
    }
}



bool OcConfiguration::showExcerpts() const { return m_showExcerpts; }

void OcConfiguration::setShowExcerpts(const bool &nShowExcerpts)
{
    if (nShowExcerpts != m_showExcerpts) {
        m_showExcerpts = nShowExcerpts;
        conf->setSetting("display/excerpts", QDBusVariant(showExcerpts()));
        emit showExcerptsChanged(showExcerpts());
    }
}


bool OcConfiguration::showPicturesInList() const { return m_showPicturesInList; }

void OcConfiguration::setShowPicturesInList(const bool &nShowPicturesInList)
{
    if (nShowPicturesInList != m_showPicturesInList) {
        m_showPicturesInList = nShowPicturesInList;
        conf->setSetting("display/picturesInList", QDBusVariant(showPicturesInList()));
        emit showPicturesInListChanged(showPicturesInList());
    }
}



int OcConfiguration::updateBehavior() const { return m_updateBehavior; }

void OcConfiguration::setUpdateBehavior(const int &nUpdateBehavior)
{
    if (nUpdateBehavior != m_updateBehavior) {
        m_updateBehavior = nUpdateBehavior;
        conf->setSetting("update/behavior", QDBusVariant(updateBehavior()));
        emit updateBehaviorChanged(updateBehavior());
    }
}


int OcConfiguration::updateInterval() const { return m_updateInterval; }

void OcConfiguration::setUpdateInterval(const int &nUpdateInterval)
{
    if (nUpdateInterval != m_updateInterval) {
        m_updateInterval = nUpdateInterval;
        conf->setSetting("update/interval", QDBusVariant(updateInterval()));
        emit updateIntervalChanged(updateInterval());
    }
}


QString OcConfiguration::eventFeeds() const { return m_eventFeeds; }

void OcConfiguration::setEventFeeds(const QString &nEventFeeds)
{
    if (nEventFeeds != m_eventFeeds) {
        m_eventFeeds = nEventFeeds;
        conf->setSetting("event/feeds", QDBusVariant(eventFeeds()));
        emit eventFeedsChanged(eventFeeds());
    }
}


bool OcConfiguration::quitEngine() const { return m_quitEngine; }

void OcConfiguration::setQuitEngine(const bool &nQuitEngine)
{
    if (nQuitEngine != m_quitEngine) {
        m_quitEngine = nQuitEngine;
        conf->setSetting("engine/quitonclose", QDBusVariant(quitEngine()));
        emit quitEngineChanged(quitEngine());
    }
}


bool OcConfiguration::notifyFeedsFolders() const { return m_notifyFeedsFolders; }

void OcConfiguration::setNotifyFeedsFolders(const bool &nNotifyFeedsFolders)
{
    if (nNotifyFeedsFolders != m_notifyFeedsFolders) {
        m_notifyFeedsFolders = nNotifyFeedsFolders;
        conf->setSetting("notifications/feedsFolders", QDBusVariant(notifyFeedsFolders()));
        emit notifyFeedsFoldersChanged(notifyFeedsFolders());
    }
}



bool OcConfiguration::notifyNewItems() const { return m_notifyNewItems; }

void OcConfiguration::setNotifyNewItems(const bool &nNotifyNewItems)
{
    if (nNotifyNewItems != m_notifyNewItems) {
        m_notifyNewItems = nNotifyNewItems;
        conf->setSetting("notifications/newItems", QDBusVariant(notifyNewItems()));
        emit notifyNewItemsChanged(notifyNewItems());
    }
}



bool OcConfiguration::privacyShown() const { return m_privacyShown; }

void OcConfiguration::setPrivacyShown(const bool &nPrivacyShown)
{
    if (nPrivacyShown != m_privacyShown) {
        m_privacyShown = nPrivacyShown;
        conf->setSetting("display/privacypolicy", QDBusVariant(privacyShown()));
        emit privacyShownChanged(privacyShown());
    }
}


bool OcConfiguration::isValid() const { return m_isValid; }

void OcConfiguration::setIsValid(const bool &nIsValid)
{
    if (nIsValid != m_isValid) {
        m_isValid = nIsValid;
        emit isValidChanged(isValid());
    }
}


bool OcConfiguration::createLogFile() const { return m_createLogFile; }

void OcConfiguration::setCreateLogFile(const bool &nCreateLogFile)
{
    if (nCreateLogFile != m_createLogFile) {
        m_createLogFile = nCreateLogFile;
        conf->setSetting("support/createLogFile", QDBusVariant(createLogFile()));
        emit createLogFileChanged(createLogFile());
    }
}


QString OcConfiguration::displayLanguage() const { return m_displayLanguage; }

void OcConfiguration::setDisplayLanguage(const QString &nDisplayLanguage)
{
    if (nDisplayLanguage != m_displayLanguage) {
        m_displayLanguage = nDisplayLanguage;
        conf->setSetting("display/language", QDBusVariant(displayLanguage()));
        emit displayLanguageChanged(displayLanguage());
    }
}



bool OcConfiguration::directLinkOpening() const { return m_directLinkOpening; }

void OcConfiguration::setDirectLinkOpening(const bool &nDirectLinkOpening)
{
    if (nDirectLinkOpening != m_directLinkOpening) {
        m_directLinkOpening = nDirectLinkOpening;
        conf->setSetting("display/directLinkOpening", QDBusVariant(directLinkOpening()));
        emit directLinkOpeningChanged(directLinkOpening());
    }
}




#if !defined(MEEGO_EDITION_HARMATTAN)

bool OcConfiguration::accountEnabled() const { return m_accountEnabled; }

void OcConfiguration::setAccountEnabled(const bool &nAccountEnabled)
{
    if (nAccountEnabled != m_accountEnabled) {
        m_accountEnabled = nAccountEnabled;
        conf->setSetting("account/enabled", QDBusVariant(accountEnabled()));
        emit accountEnabledChanged(accountEnabled());
    }
}


QString OcConfiguration::accountUser() const { return m_accountUser; }

void OcConfiguration::setAccountUser(const QString &nAccountUser)
{
    if (nAccountUser != m_accountUser) {
        m_accountUser = nAccountUser;
        setIsValid(accountUser() != "" && accountPassword() !=  "" && accountServer() != "");
        conf->setSetting("account/user", QDBusVariant(accountUser()));
        emit accountUserChanged(accountUser());
    }
}


QString OcConfiguration::accountPassword() const { return m_accountPassword; }

void OcConfiguration::setAccountPassword(const QString &nAccountPassword)
{
    if (nAccountPassword != m_accountPassword) {
        m_accountPassword = nAccountPassword;
        setIsValid(accountUser() != "" && accountPassword() !=  "" && accountServer() != "");
        conf->setSetting("account/password", QDBusVariant(accountPassword()));
        emit accountPasswordChanged(accountPassword());
    }
}


QString OcConfiguration::accountServer() const { return m_accountServer; }

void OcConfiguration::setAccountServer(const QString &nAccountServer)
{
    if (nAccountServer != m_accountServer) {
        m_accountServer = nAccountServer;
        setIsValid(accountUser() != "" && accountPassword() !=  "" && accountServer() != "");
        conf->setSetting("account/server", QDBusVariant(accountServer()));
        emit accountServerChanged(accountServer());
    }
}


bool OcConfiguration::accountUseSSL() const { return m_accountUseSSL; }

void OcConfiguration::setAccountUseSSL(const bool &nAccountUseSSL)
{
    if (nAccountUseSSL != m_accountUseSSL) {
        m_accountUseSSL = nAccountUseSSL;
        conf->setSetting("account/usessl", QDBusVariant(accountUseSSL()));
        emit accountUseSSLChanged(accountUseSSL());
    }
}


bool OcConfiguration::accountIgnoreSSLErrors() const { return m_accountIgnoreSSLErrors; }

void OcConfiguration::setAccountIgnoreSSLErrors(const bool &nAccountIgnoreSSLErrors)
{
    if (nAccountIgnoreSSLErrors != m_accountIgnoreSSLErrors) {
        m_accountIgnoreSSLErrors = nAccountIgnoreSSLErrors;
        conf->setSetting("account/ignoresslerrors", QDBusVariant(accountIgnoreSSLErrors()));
        emit accountIgnoreSSLErrorsChanged(accountIgnoreSSLErrors());
    }
}
#else


bool OcConfiguration::useRichText() const { return m_useRichText; }

void OcConfiguration::setUseRichText(const bool &nUseRichText)
{
    if (nUseRichText != m_useRichText) {
        m_useRichText = nUseRichText;
        conf->setSetting("display/richText", QDBusVariant(useRichText()));
        emit useRichTextChanged(useRichText());
    }
}


bool OcConfiguration::themeInverted() const { return m_themeInverted; }

void OcConfiguration::setThemeInverted(const bool &nThemeInverted)
{
    if (nThemeInverted != m_themeInverted) {
        m_themeInverted = nThemeInverted;
        conf->setSetting("display/themeInverted", QDBusVariant(themeInverted()));
        emit themeInvertedChanged(themeInverted());
    }
}


#endif



void OcConfiguration::configReset()
{
    QVariantMap config = conf->getConfig();
    setPrivateBrowsing(config["privateBrowsing"].toBool());
    setEnableCookies(config["enableCookies"].toBool());
    setMaxItems(config["maxitems"].toInt());
    setViewMode(config["viewmode"].toInt());
    setOrderBy(config["orderby"].toString());
    setHandleImgs(config["handleimgs"].toInt());
    setHandleRead(config["handleread"].toInt());
    setSortAsc(config["sortasc"].toBool());
    setFontSize(config["fontsize"].toInt());
    setHideReadFeeds(config["hidereadfeeds"].toBool());
    setShowExcerpts(config["showExcerpts"].toBool());
    setShowPicturesInList(config["showPicturesInList"].toBool());
    setUpdateBehavior(config["updatebehavior"].toInt());
    setUpdateInterval(config["updateinterval"].toInt());
    setEventFeeds(config["eventfeeds"].toString());
    setQuitEngine(config["quitengine"].toBool());
    setNotifyFeedsFolders(config["notifyFeedsFolders"].toBool());
    setNotifyNewItems(config["notifyNewItems"].toBool());
    setIsValid(conf->isConfigSet());
#if !defined(MEEGO_EDITION_HARMATTAN)
    setAccountEnabled(config["enabled"].toBool());
    setAccountUser(config["uname"].toString());
    setAccountPassword(config["pword"].toString());
    setAccountServer(config["server"].toString());
    setAccountUseSSL(config["usessl"].toBool());
    setAccountIgnoreSSLErrors(config["ignoresslerrors"].toBool());
#else
    setUseRichText(config["userichtext"].toBool());
    setThemeInverted(config["themeinverted"].toBool());
#endif
}
