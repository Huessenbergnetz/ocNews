#ifndef OCITEMOBJECT_H
#define OCITEMOBJECT_H

#include <QString>
#include <QMetaType>

class OcItemObject {
public:
    OcItemObject();
    OcItemObject(const OcItemObject &other);
    ~OcItemObject();

    OcItemObject(const int &id, const QString &tit, const uint &pub, const QString &encLink, const QString &encMime, const bool &unr, const bool &star, const QString &u, const QString &hash, const QString &exc, const QString &img, const QString &feed, const int &fId);

        int itemId;
        QString title;
        uint pubDate;
        QString enclosureLink;
        QString enclosureMime;
        bool unread;
        bool starred;
        QString url;
        QString guidHash;
        QString feedName;
        int feedId;
        QString image;
        QString excerpt;
};

Q_DECLARE_METATYPE(OcItemObject)

#endif // OCITEMOBJECT_H
