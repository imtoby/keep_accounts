#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>

#include "src/TypeManager.h"
#include "src/TypeItem.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    TypeManager* typeManager = new TypeManager(&app);

    engine.rootContext()->setContextProperty("typeManager", typeManager);

    qRegisterMetaType<TypeItem>("TypeItem");
    qmlRegisterType<TypeItem>("com.imtoby.keep_accounts", 1, 0, "TypeItem");

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
