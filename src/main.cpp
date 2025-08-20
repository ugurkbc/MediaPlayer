#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <ui/videoitem.h>
#include <utils.h>
#include <streamcontrol/videocontrol/videocontrol.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    gst_init(&argc, &argv);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<Utils>("Utils", 1, 0, "Utils", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Utils::getInstance();});

    qmlRegisterType<VideoItem>("App", 1, 0, "VideoItem");
    qmlRegisterType<VideoControl>("App", 1, 0, "VideoControl");

    const QUrl url(QStringLiteral("qrc:/qml/Main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
