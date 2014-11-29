#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include "serialtest.h"
#include <QtQml>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("serialtest",new SerialTest);
    viewer.setMainQmlFile(QStringLiteral("qml/TestSerial/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
