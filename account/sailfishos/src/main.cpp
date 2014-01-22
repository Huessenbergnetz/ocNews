//#ifdef QT_QML_DEBUG
#include <QtQuick>
//#endif

#include <sailfishapp.h>
#include "../../common/globals.h"
#include "ocdbusinterface.h"

int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/template.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    QGuiApplication* app = SailfishApp::application(argc, argv);

    QQuickView* view = SailfishApp::createView();

    OcDBusInterface dbus;

    view->rootContext()->setContextProperty("dbus", &dbus);

    view->setSource(QUrl::fromLocalFile("/usr/share/harbour-ocnews-account/qml/harbour-ocnews-account.qml"));
    view->show();

    return app->exec();
}
