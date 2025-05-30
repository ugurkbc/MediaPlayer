#ifndef GSTREAMERVIDEOCAPTURE_H
#define GSTREAMERVIDEOCAPTURE_H

#include <QThread>
#include <QString>
#include <QElapsedTimer>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>

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
    static gboolean busCallback(GstBus* bus, GstMessage* message, gpointer data);
    static GstFlowReturn newBufferCallback(GstElement* appsink, gpointer data);
protected:
    void run() override;
private:
    QString pipelineString;
    QElapsedTimer elapsedTimer;
    GstElement* pipeline;
    GstBus* bus;
    GstElement* appsink;
    GMainLoop* loop;

    gint width;
    gint height;
    double frameRate;
    int fpsNum = 0;
    int fpsDen = 1;
    bool paused;
    int frameCount;
    int frameIntervalMs;

signals:
    void newImage(QImage &);
};

#endif // GSTREAMERVIDEOCAPTURE_H
