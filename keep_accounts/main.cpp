#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>

#include "src/InfoManager.h"
#include "src/TypeModel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterUncreatableType<TypeModel, 1>("com.toby.keep_accounts", 1, 0,
                                             "TypeModel",
                                             "Cannot create TypeModel.");

    InfoManager* infoManager = new InfoManager(&app);

    engine.rootContext()->setContextProperty("infoManager", infoManager);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
