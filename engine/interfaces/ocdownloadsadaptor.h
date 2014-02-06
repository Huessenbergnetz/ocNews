/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -c OcDownloadManagerAdaptor -a ocdownloadsadaptor.h:ocdownloadsadaptor.cpp de.buschmann23.ocNewsEngine.Downloads.xml
 *
 * qdbusxml2cpp is Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef OCDOWNLOADSADAPTOR_H
#define OCDOWNLOADSADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface de.buschmann23.ocNewsEngine.Downloads
 */
class OcDownloadManagerAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "de.buschmann23.ocNewsEngine.Downloads")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"de.buschmann23.ocNewsEngine.Downloads\">\n"
"    <method name=\"append\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"id\"/>\n"
"    </method>\n"
"    <method name=\"getCurrentItem\">\n"
"      <arg direction=\"out\" type=\"s\"/>\n"
"    </method>\n"
"    <method name=\"itemExists\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"link\"/>\n"
"      <arg direction=\"out\" type=\"s\"/>\n"
"    </method>\n"
"    <method name=\"itemInQueue\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"id\"/>\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"    </method>\n"
"    <method name=\"abortDownload\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"id\"/>\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"    </method>\n"
"    <method name=\"deleteFile\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"link\"/>\n"
"      <arg direction=\"out\" type=\"b\"/>\n"
"    </method>\n"
"    <signal name=\"finished\"/>\n"
"    <signal name=\"finishedFile\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"finishedFileId\"/>\n"
"    </signal>\n"
"    <signal name=\"started\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"startedFileId\"/>\n"
"    </signal>\n"
"    <signal name=\"progress\">\n"
"      <arg direction=\"out\" type=\"i\" name=\"rec\"/>\n"
"      <arg direction=\"out\" type=\"i\" name=\"tot\"/>\n"
"    </signal>\n"
"    <signal name=\"enqueued\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"enqueuedId\"/>\n"
"    </signal>\n"
"  </interface>\n"
        "")
public:
    OcDownloadManagerAdaptor(QObject *parent);
    virtual ~OcDownloadManagerAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    bool abortDownload(const QString &id);
    void append(const QString &id);
    bool deleteFile(const QString &link);
    QString getCurrentItem();
    QString itemExists(const QString &link);
    bool itemInQueue(const QString &id);
Q_SIGNALS: // SIGNALS
    void enqueued(const QString &enqueuedId);
    void finished();
    void finishedFile(const QString &finishedFileId);
    void progress(int rec, int tot);
    void started(const QString &startedFileId);
};

#endif
