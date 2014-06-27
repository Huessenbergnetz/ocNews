#ifndef COCOMBINEDOBJECT_H
#define COCOMBINEDOBJECT_H

#include <QString>

class OcCombinedObject {
public:
    OcCombinedObject(const int &itemId, const int &ty, const QString &tit, const int &uC, const QString iS, const int &iW, const int &iH, const int &fId, const QString &fName):
        id(itemId),
        type(ty),
        title(tit),
        unreadCount(uC),
        iconSource(iS),
        iconWidth(iW),
        iconHeight(iH),
        folderId(fId),
        folderName(fName){}
    int id;
    int type;
    QString title;
    int unreadCount;
    QString iconSource;
    int iconWidth;
    int iconHeight;
    int folderId;
    QString folderName;
};

#endif // COCOMBINEDOBJECT_H
