#ifndef OCCONFIGURATION_H
#define OCCONFIGURATION_H

#include <QObject>
#include <QVariantMap>
#include "dbus/interfaces/occonfigif.h"

class OcConfiguration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool privateBrowsing READ privateBrowsing WRITE setPrivateBrowsing NOTIFY privateBrowsingChanged)
    Q_PROPERTY(bool enableCookies READ enableCookies WRITE setEnableCookies NOTIFY enableCookiesChanged)
    Q_PROPERTY(int maxItems READ maxItems WRITE setMaxItems NOTIFY maxItemsChanged)
    Q_PROPERTY(int viewMode READ viewMode WRITE setViewMode NOTIFY viewModeChanged)
    Q_PROPERTY(QString orderBy READ orderBy WRITE setOrderBy NOTIFY orderByChanged)
    Q_PROPERTY(int handleImgs READ handleImgs WRITE setHandleImgs NOTIFY handleImgsChanged)
    Q_PROPERTY(int handleRead READ handleRead WRITE setHandleRead NOTIFY handleReadChanged)
    Q_PROPERTY(bool sortAsc READ sortAsc WRITE setSortAsc NOTIFY sortAscChanged)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(bool hideReadFeeds READ hideReadFeeds WRITE setHideReadFeeds NOTIFY hideReadFeedsChanged)
    Q_PROPERTY(bool showExcerpts READ showExcerpts WRITE setShowExcerpts NOTIFY showExcerptsChanged)
    Q_PROPERTY(bool showPicturesInList READ showPicturesInList WRITE setShowPicturesInList NOTIFY showPicturesInListChanged)
    Q_PROPERTY(int updateBehavior READ updateBehavior WRITE setUpdateBehavior NOTIFY updateBehaviorChanged)
    Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)
    Q_PROPERTY(QString eventFeeds READ eventFeeds WRITE setEventFeeds NOTIFY eventFeedsChanged)
    Q_PROPERTY(bool quitEngine READ quitEngine WRITE setQuitEngine NOTIFY quitEngineChanged)
    Q_PROPERTY(bool notifyFeedsFolders READ notifyFeedsFolders WRITE setNotifyFeedsFolders NOTIFY notifyFeedsFoldersChanged)
    Q_PROPERTY(bool notifyNewItems READ notifyNewItems WRITE setNotifyNewItems NOTIFY notifyNewItemsChanged)
    Q_PROPERTY(bool isValid READ isValid WRITE setIsValid NOTIFY isValidChanged)
    Q_PROPERTY(bool privacyShown READ privacyShown WRITE setPrivacyShown NOTIFY privacyShownChanged)
    Q_PROPERTY(int displayedVersion READ displayedVersion WRITE setDisplayedVersion NOTIFY displayedVersionChanged)
#if !defined(MEEGO_EDITION_HARMATTAN)
    Q_PROPERTY(bool accountEnabled READ accountEnabled WRITE setAccountEnabled NOTIFY accountEnabledChanged)
    Q_PROPERTY(QString accountUser READ accountUser WRITE setAccountUser NOTIFY accountUserChanged)
    Q_PROPERTY(QString accountPassword READ accountPassword WRITE setAccountPassword NOTIFY accountPasswordChanged)
    Q_PROPERTY(QString accountServer READ accountServer WRITE setAccountServer NOTIFY accountServerChanged)
    Q_PROPERTY(bool accountUseSSL READ accountUseSSL WRITE setAccountUseSSL NOTIFY accountUseSSLChanged)
    Q_PROPERTY(bool accountIgnoreSSLErrors READ accountIgnoreSSLErrors WRITE setAccountIgnoreSSLErrors NOTIFY accountIgnoreSSLErrorsChanged)
#else
    Q_PROPERTY(bool useRichText READ useRichText WRITE setUseRichText NOTIFY useRichTextChanged)
    Q_PROPERTY(bool themeInverted READ themeInverted WRITE setThemeInverted NOTIFY themeInvertedChanged)
#endif
public:
    explicit OcConfiguration(QObject *parent = 0);

    bool privateBrowsing() const;
    bool enableCookies() const;
    int maxItems() const;
    int viewMode() const;
    QString orderBy() const;
    int handleImgs() const;
    int handleRead() const;
    bool sortAsc() const;
    int fontSize() const;
    bool hideReadFeeds() const;
    bool showExcerpts() const;
    bool showPicturesInList() const;
    int updateBehavior() const;
    int updateInterval() const;
    QString eventFeeds() const;
    bool quitEngine() const;
    bool notifyFeedsFolders() const;
    bool notifyNewItems() const;
    bool isValid() const;
    bool privacyShown() const;
    int displayedVersion() const;
#if !defined(MEEGO_EDITION_HARMATTAN)
    bool accountEnabled() const;
    QString accountUser() const;
    QString accountPassword() const;
    QString accountServer() const;
    bool accountUseSSL() const;
    bool accountIgnoreSSLErrors() const;
#else
    bool useRichText() const;
    bool themeInverted() const;
#endif

signals:
    void privateBrowsingChanged(const bool &nPrivateBrowsing);
    void enableCookiesChanged(const bool &nEnableCookies);
    void maxItemsChanged(const int &nMaxItems);
    void viewModeChanged(const int &nViewMode);
    void orderByChanged(const QString &nOrderBy);
    void handleImgsChanged(const int &nHandleImgs);
    void handleReadChanged(const int &nHandleRead);
    void sortAscChanged(const bool &nSortAsc);
    void fontSizeChanged(const int &nFontSize);
    void hideReadFeedsChanged(const bool &nHideReadFeeds);
    void showExcerptsChanged(const bool &nShowExcerpts);
    void showPicturesInListChanged(const bool &nShowPicturesInList);
    void updateBehaviorChanged(const int &nUpdateBehavior);
    void updateIntervalChanged(const int &nUpdateInterval);
    void eventFeedsChanged(const QString &nEventFeeds);
    void quitEngineChanged(const bool &nQuitEngine);
    void notifyFeedsFoldersChanged(const bool &nNotifyFeedsFolders);
    void notifyNewItemsChanged(const bool &nNotifyNewItems);
    void isValidChanged(const bool &nIsValid);
    void privacyShownChanged(const bool &nPrivacyShown);
    void displayedVersionChanged(const int &nDisplayedVersion);
#if !defined(MEEGO_EDITION_HARMATTAN)
    void accountEnabledChanged(const bool &nAccountEnabled);
    void accountUserChanged(const QString &nAccountUser);
    void accountPasswordChanged(const QString &nAccountPassword);
    void accountServerChanged(const QString &nAccountServer);
    void accountUseSSLChanged(const bool &nAccountUseSSL);
    void accountIgnoreSSLErrorsChanged(const bool &nAccountIgnoreSSLErrors);
#else
    void useRichTextChanged(const bool &nUseRichText);
    void themeInvertedChanged(const bool &nThemeInverted);
#endif

public slots:
    void setPrivateBrowsing(const bool &nPrivateBrowsing);
    void setEnableCookies(const bool &nEnableCookies);
    void setMaxItems(const int &nMaxItems);
    void setViewMode(const int &nViewMode);
    void setOrderBy(const QString &nOrderBy);
    void setHandleImgs(const int &nHandleImgs);
    void setHandleRead(const int &nHandleRead);
    void setSortAsc(const bool &nSortAsc);
    void setFontSize(const int &nFontSize);
    void setHideReadFeeds(const bool &nHideReadFeeds);
    void setShowExcerpts(const bool &nShowExcerpts);
    void setShowPicturesInList(const bool &nShowPicturesInList);
    void setUpdateBehavior(const int &nUpdateBehavior);
    void setUpdateInterval(const int &nUpdateInterval);
    void setEventFeeds(const QString &nEventFeeds);
    void setQuitEngine(const bool &nQuitEngine);
    void setNotifyFeedsFolders(const bool &nNotifyFeedsFolders);
    void setNotifyNewItems(const bool &nNotifyNewItems);
    void setIsValid(const bool &nIsValid);
    void setPrivacyShown(const bool &nPrivacyShown);
    void setDisplayedVersion(const int &nDisplayedVersion);
#if !defined(MEEGO_EDITION_HARMATTAN)
    void setAccountEnabled(const bool &nAccountEnabled);
    void setAccountUser(const QString &nAccountUser);
    void setAccountPassword(const QString &nAccountPassword);
    void setAccountServer(const QString &nAccountServer);
    void setAccountUseSSL(const bool &nAccountUseSSL);
    void setAccountIgnoreSSLErrors(const bool &nAccountIgnoreSSLErrors);
#else
    void setUseRichText(const bool &nUseRichText);
    void setThemeInverted(const bool &nThemeInverted);
#endif

public slots:
    void configReset();

private:
    de::buschmann23::ocNewsEngine::Configuration *conf;

    bool m_privateBrowsing;
    bool m_enableCookies;
    int m_maxItems;
    int m_viewMode;
    QString m_orderBy;
    int m_handleImgs;
    int m_handleRead;
    bool m_sortAsc;
    int m_fontSize;
    bool m_hideReadFeeds;
    bool m_showExcerpts;
    bool m_showPicturesInList;
    int m_updateBehavior;
    int m_updateInterval;
    QString m_eventFeeds;
    bool m_quitEngine;
    bool m_notifyFeedsFolders;
    bool m_notifyNewItems;
    bool m_isValid;
    bool m_privacyShown;
    int m_displayedVersion;
#if !defined(MEEGO_EDITION_HARMATTAN)
    bool m_accountEnabled;
    QString m_accountUser;
    QString m_accountPassword;
    QString m_accountServer;
    bool m_accountUseSSL;
    bool m_accountIgnoreSSLErrors;
#else
    bool m_useRichText;
    bool m_themeInverted;
#endif
};

#endif // OCCONFIGURATION_H
