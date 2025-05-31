#ifndef GSTREAMERVIDEOCAPTURE_H
#define GSTREAMERVIDEOCAPTURE_H

#include <QThread>
#include <QString>
#include <QElapsedTimer>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include "streamcontrol/videocontrol/util/util.h"

class GstreamerVideoCapture : public QThread
{
    Q_OBJECT
public:
    explicit GstreamerVideoCapture(QObject *parent = nullptr);
    ~GstreamerVideoCapture();
public:
    void play(QString str);
    void play();
    void pause();
    void close();
private:
    void init();
    void clean();
    bool getVideoFeaturesFromFileLocation(const QString &fileLocation);
    static gboolean busCallback(GstBus* bus, GstMessage* message, gpointer data);
    static GstFlowReturn newBufferCallbackFileSource(GstElement* appsink, gpointer data);
    static GstFlowReturn newBufferCallbackRealTime(GstElement* appsink, gpointer data);
    static QString extractFilePathFromPipeline(const QString &pipeline);
protected:
    void run() override;
private:
    QString pipelineString;
    GstElement* pipeline = nullptr;
    GstBus* bus = nullptr;
    GstElement* appsink = nullptr;
    GMainLoop* loop = nullptr;

    using CallbackFunc = GstFlowReturn (*)(GstElement*, gpointer);

    CallbackFunc bufferCallbacks[static_cast<unsigned int>(VideoSource::SIZE)] = {0};
    gint width = 0;
    gint height = 0;
    bool paused = 0;
    int frameCount = 0;
    qint64 frameIntervalMs = 0;
    float frameRate = 0;
    int delayValue = 0;
    QElapsedTimer elapsedTimer;

signals:
    void newImage(QImage &);
};

#endif // GSTREAMERVIDEOCAPTURE_H
