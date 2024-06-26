#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <Gstreamer/gstreamervideocapture.h>
#include <videoitem.h>
#include <Utils/utils.h>
#include <StreamControl/VideoControl/videocontrol.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    gst_init(&argc, &argv);

    QQmlApplicationEngine engine;

    VideoControl videoControl;
    engine.rootContext()->setContextProperty("videoControl", &videoControl);

    qmlRegisterSingletonType<Utils>("Utils", 1, 0, "Utils", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return Utils::getInstance();
    });

    qmlRegisterType<VideoItem>("VideoItem", 1, 0, "VideoItem");

    const QUrl url(u"qrc:/MediaPlayer/Main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url, &videoControl](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);

            VideoItem *videoItem = obj->findChild<VideoItem *>("videoItem");

            if (videoItem) {
                videoControl.setVideoItem(videoItem);
            }
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
