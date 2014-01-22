#include "ocshareui.h"

OcShareUi::OcShareUi(QObject *parent) :
    QObject(parent)
{
}


void OcShareUi::share(const QString &link, const QString &title, const QString &description)
{
    MDataUri dataUri;

    dataUri.setMimeType("text/x-url");

    dataUri.setTextData(link);

    dataUri.setAttribute("title", title);

    dataUri.setAttribute("description", description);

    if (dataUri.isValid() == false) {
        qCritical() << "Invalid URI";

        return;
    }

    QStringList items;

    items << dataUri.toString();

    ShareUiInterface shareInterface("com.nokia.ShareUi");

    if (shareInterface.isValid())
    {
        shareInterface.share(items);
    } else {
        qCritical() << "Invalid Interface";

        return;
    }
}
