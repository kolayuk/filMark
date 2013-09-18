#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include <app.h>
#include <localizer.h>
#include <QDeclarativeContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    App* a=new App();
    QStringList strings=a->locs->GetStringsAsList();
    QmlApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("strings", QVariant::fromValue(strings));
    viewer.rootContext()->setContextProperty("application", a);
    viewer.rootContext()->setContextProperty("serverlaunched", a->IsServerWorking());
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);
    viewer.setMainQmlFile(QLatin1String("qml/videoBookmark/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
