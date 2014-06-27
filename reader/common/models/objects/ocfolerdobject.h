#ifndef OCFOLERDOBJECT_H
#define OCFOLERDOBJECT_H

#include <QString>

class OcFolderObject {
public:
    OcFolderObject(const int &itemId, const int &ty, const QString &tit, const int &uC, const QString iS, const int &iW, const int &iH, const int &fC):
        id(itemId),
        type(ty),
        title(tit),
        unreadCount(uC),
        iconSource(iS),
        iconWidth(iW),
        iconHeight(iH),
        feedCount(fC){}
    int id;
    int type;
    QString title;
    int unreadCount;
    QString iconSource;
    int iconWidth;
    int iconHeight;
    int feedCount;
};

#endif // OCFOLERDOBJECT_H
