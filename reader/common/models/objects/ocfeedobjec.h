#ifndef OCFEEDOBJEC_H
#define OCFEEDOBJEC_H

#include <QString>

class OcFeedObject {
public:
    OcFeedObject(const int &itemId, const int &ty, const QString &tit, const int &uC, const QString iS, const int &iW, const int &iH):
        id(itemId),
        type(ty),
        title(tit),
        unreadCount(uC),
        iconSource(iS),
        iconWidth(iW),
        iconHeight(iH){}
    int id;
    int type;
    QString title;
    int unreadCount;
    QString iconSource;
    int iconWidth;
    int iconHeight;
};

#endif // OCFEEDOBJEC_H
