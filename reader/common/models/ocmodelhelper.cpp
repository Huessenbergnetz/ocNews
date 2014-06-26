#include "ocmodelhelper.h"

OcModelHelper::OcModelHelper(QObject *parent) :
    QObject(parent)
{
}


QString OcModelHelper::niceTime(const uint &t) const
{
    QString date = (QDateTime::currentDateTimeUtc().date() == QDateTime::fromTime_t(t).date()) ?
                                    tr("Today") :
                                    QDateTime::fromTime_t(t).toLocalTime().toString(tr("d. MMMM"));
    QString time = QDateTime::fromTime_t(t).toLocalTime().toString(tr("hh:mm"));

    return date + " | " + time;
}


QString OcModelHelper::prepareBody(const QString &b) const
{
    QString newBody = b;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    return newBody.remove(QRegularExpression("<[^>]*>")).trimmed().left(200);
#else
    return newBody.remove(QRegExp("<[^>]*>")).trimmed().left(200);
#endif
}
