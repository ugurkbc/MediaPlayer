#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <Gstreamer/gstreamervideocapture.h>
#include <videoitem.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    gst_init(&argc, &argv);

    const char *pipeline = "filesrc location=C:/Users/ugurk/Videos/video.mp4 ! qtdemux ! h264parse ! avdec_h264 ! videoconvert ! video/x-raw,format=RGB ! appsink name=sink";

    QQmlApplicationEngine engine;

    qmlRegisterType<VideoItem>("VideoItem", 1, 0, "VideoItem");

    const QUrl url(u"qrc:/MediaPlayer/Main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
